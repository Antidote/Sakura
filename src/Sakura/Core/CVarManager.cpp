#include "Sakura/Core/CVarManager.hpp"
#include "Sakura/Core/CVar.hpp"
#include "Sakura/Core/Keys.hpp"

namespace Sakura
{
namespace Core
{
CVar* com_developer;

CVarManager::CVarManager()
{
    com_developer = new CVar("developer", "false", "Enables developer mode", CVar::Boolean, (CVar::System | CVar::Cheat | CVar::Archive));

    registerCVar(com_developer);

    loadFromFile();
}

CVarManager::~CVarManager()
{
    for (std::pair<std::string, CVar*> pair : m_cvars)
    {
        delete pair.second;
        pair.second = NULL;
    }

    m_cvars.clear();
}

bool CVarManager::registerCVar(CVar *cvar)
{
    if (m_cvars.find(cvar->name()) != m_cvars.end())
        return false;

    m_cvars[cvar->name()] = cvar;
    cvar->deserialize(m_config.RootElement());
    return true;
}

CVar* CVarManager::findCVar(const std::string &name)
{
    if (m_cvars.find(name) == m_cvars.end())
        return NULL;
    return m_cvars[name];
}

std::vector<CVar*> CVarManager::archivedCVars()
{
    std::vector<CVar*> ret;
    for (std::pair<std::string, CVar*> pair : m_cvars)
    {
        if ((pair.second->flags() & CVar::Archive) == CVar::Archive)
            ret.push_back(pair.second);
    }

    return ret;
}

std::vector<CVar *> CVarManager::bindings()
{
    std::vector<CVar*> ret;
    for (std::pair<std::string, CVar*> pair : m_cvars)
    {
        if (pair.second->type() == CVar::Bind)
            ret.push_back(pair.second);
    }

    return ret;
}

void CVarManager::writeToFile(const std::string &file)
{
    bool oldDeveloper = com_developer->toBoolean();
    com_developer->fromBoolean(false);

    bool newConfig = m_config.NoChildren();

    TiXmlNode* root = NULL;
    if (newConfig)
    {
        TiXmlDeclaration* versionDecl = new TiXmlDeclaration("1.0", "UTF-8", "");
        root = new TiXmlElement("Config");
        m_config.LinkEndChild(versionDecl);
        m_config.LinkEndChild(root);
    }
    else
        root = m_config.RootElement();

    for (CVar* binding : bindings())
        binding->serializeBinding(root);

    for (CVar* cvar : archivedCVars())
    {
        cvar->serializeCVar(root, oldDeveloper);
    }

    com_developer->fromBoolean(oldDeveloper);
    m_config.SaveFile(file.c_str());
}

void CVarManager::loadFromFile(const std::string& file)
{
    m_config.LoadFile(file.c_str());
}

} // Core
} // Sakura

