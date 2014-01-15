#include "Sakura/Core/CVarManager.hpp"
#include "Sakura/Core/CVar.hpp"
#include "Sakura/Core/Keys.hpp"
#include <utility.hpp>

namespace Sakura
{
namespace Core
{
CVar* com_developer = NULL;
CVar* com_configfile;

CVarManager::CVarManager()
{
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

void CVarManager::initialize()
{
    com_developer = new CVar("developer", "false", "Enables developer mode", CVar::Boolean, (CVar::System | CVar::Cheat | CVar::ReadOnly));
    com_configfile = new CVar("config", "config.cfg", "File to store configuration", CVar::Literal, CVar::System);
    loadFromFile();
}

bool CVarManager::registerCVar(CVar *cvar)
{
    std::string tmp = cvar->name();
    zelda::utility::tolower(tmp);
    if (m_cvars.find(tmp) != m_cvars.end())
        return false;

    m_cvars[tmp] = cvar;
    if (m_config.RootElement())
        cvar->deserialize(m_config.RootElement());

    return true;
}

void CVarManager::requestSerialization(CVar* cvar)
{
    bool oldDeveloper = suppressDeveloper();

    if (cvar != NULL)
    {
        if (cvar->type() == CVar::Bind)
            cvar->serializeBinding(m_config.RootElement());
        else
            cvar->serializeCVar(m_config.RootElement(), oldDeveloper);
    }

    restoreDeveloper(oldDeveloper);
    writeToFile();
}

CVar* CVarManager::findCVar(const std::string &name)
{
    std::string tmp = std::string(name);
    zelda::utility::tolower(tmp);
    if (m_cvars.find(tmp) == m_cvars.end())
        return NULL;
    return m_cvars[tmp];
}

std::vector<CVar*> CVarManager::archivedCVars() const
{
    std::vector<CVar*> ret;
    for (std::pair<std::string, CVar*> pair : m_cvars)
    {
        if ((pair.second->flags() & CVar::Archive) == CVar::Archive && pair.second->type() != CVar::Bind)
            ret.push_back(pair.second);
    }

    return ret;
}

std::vector<CVar *> CVarManager::bindings() const
{
    std::vector<CVar*> ret;
    for (std::pair<std::string, CVar*> pair : m_cvars)
    {
        if (pair.second->type() == CVar::Bind)
            ret.push_back(pair.second);
    }

    return ret;
}

std::vector<CVar*> CVarManager::cvars() const
{
    std::vector<CVar*> ret;
    for (std::pair<std::string, CVar*> pair : m_cvars)
        ret.push_back(pair.second);

    return ret;
}

void CVarManager::writeToFile(const std::string &file)
{
    if (file != std::string())
        com_configfile->fromLiteral(file);

    bool oldDeveloper = suppressDeveloper();

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
        cvar->serializeCVar(root, oldDeveloper);

    restoreDeveloper(oldDeveloper);
    m_config.SaveFile(com_configfile->toLiteral());
}

void CVarManager::loadFromFile(const std::string& file)
{
    if (file != std::string())
        com_configfile->fromLiteral(file);
    m_config.LoadFile(com_configfile->toLiteral());
    m_config.SetCondenseWhiteSpace(false);

    for (CVar* cvar : cvars())
        cvar->deserialize(m_config.RootElement());
}

bool CVarManager::suppressDeveloper()
{
    bool oldDeveloper = com_developer->toBoolean();
    com_developer->unlock();
    com_developer->fromBoolean(false);

    return oldDeveloper;
}

void CVarManager::restoreDeveloper(bool oldDeveloper)
{
    com_developer->unlock();
    com_developer->fromBoolean(oldDeveloper);
    com_developer->lock();
}

} // Core
} // Sakura

