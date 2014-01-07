#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/ResourceManager.hpp"
#include "Sakura/Resources/SongResource.hpp"
#include <physfs.h>

namespace Sakura
{
namespace Resources
{
SongResource::SongResource(const std::string& filename, bool precache)
    : ResourceBase(filename, precache),
      m_stream(filename)
{
    if (precache && exists())
    {
        sEngineRef().console().print(Core::Console::Info, "Precaching song %s@%s...", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
        load();
    }
}

SongResource::~SongResource()
{
    if (exists())
        sEngineRef().console().print(Core::Console::Info, "Song destroyed %s@%s", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
    delete m_data;
}

std::string SongResource::filename() const
{
    return m_filename;
}

sf::Music* SongResource::data()
{
    return m_data;
}

void SongResource::load()
{
    if (!exists())
        return;

    if (!m_precached)
        sEngineRef().console().print(Core::Console::Info, "Loading song %s@%s...", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());

    if (exists())
    {
        m_data = new sf::Music;
        if (m_data->openFromStream(m_stream))
        {
            m_isLoaded = true;
            sEngineRef().console().print(Core::Console::Message, "done.");
        }
        else
        {
            m_isLoaded = false;
            sEngineRef().console().print(Core::Console::Warning, "failed!");
        }
        return;
    }

    sEngineRef().console().print(Core::Console::Warning, "Song %s@%s does not exist", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
}

bool SongResource::exists() const
{
    return PHYSFS_exists(m_filename.c_str());
}

bool SongResource::isLoaded() const
{
    return m_isLoaded;
}

bool SongResource::isPrecached() const
{
    return m_precached;
}
} // Resources
} // Sakura



