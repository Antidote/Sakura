#ifndef CVARMANAGER_HPP
#define CVARMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <fstream>
#include <TextStream.hpp>
#include <tinyxml.h>

namespace Sakura
{
namespace Core
{
class CVar;

class CVarManager
{
public:
    CVarManager();
    ~CVarManager();

    bool registerCVar(CVar* cvar);

    CVar* findCVar(const std::string& name);

    std::vector<CVar*> archivedCVars();
    std::vector<CVar*> bindings();

    void writeToFile(const std::string&  file=std::string("config.cfg"));
    void loadFromFile(const std::string& file=std::string("config.cfg"));

private:
    std::unordered_map<std::string, CVar*> m_cvars;
    void writeBindings(TiXmlNode* root, bool newConfig);
    void loadBindings(TiXmlNode* root);
    TiXmlDocument m_config;
};
} // Core
} // Sakura


#endif // CVARMANAGER_HPP
