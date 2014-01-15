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

    void initialize();
    bool registerCVar(CVar* cvar);
    void requestSerialization(CVar* cvar);

    CVar* findCVar(const std::string& name);

    std::vector<CVar*> archivedCVars() const;
    std::vector<CVar*> bindings() const;
    std::vector<CVar*> cvars() const;

    void writeToFile(const std::string&  file=std::string());
    void loadFromFile(const std::string& file=std::string());

private:
    bool suppressDeveloper();
    void restoreDeveloper(bool oldDeveloper);

    std::unordered_map<std::string, CVar*> m_cvars;
    TiXmlDocument m_config;

};
} // Core
} // Sakura


#endif // CVARMANAGER_HPP
