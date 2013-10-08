#include "MusicResource.hpp"
#include "ResourceManager.hpp"
#include "Engine.hpp"
#include <physfs.h>



MusicResource::MusicResource(const std::string& filename, bool precache)
    : Resource(filename, precache),
      m_stream(filename)
{
    if (precache)
    {
        sEngineRef().console().print(Console::Info, "Precaching song %s...", m_filename.c_str());
        load();
    }
}

MusicResource::~MusicResource()
{
    sEngineRef().console().print(Console::Info, "Song destroyed %s\n", m_filename.c_str());
    delete m_data;
}

std::string MusicResource::filename() const
{
    return m_filename;
}

sf::Music* MusicResource::data()
{
    return m_data;
}

void MusicResource::load()
{
    if (!m_precached)
        sEngineRef().console().print(Console::Info, "Loading song %s...", m_filename.c_str());

    if (exists())
    {
        m_data = new sf::Music;
        if (m_data->openFromStream(m_stream))
        {
            m_isLoaded = true;
            sEngineRef().console().print(Console::Message, "done.");
        }
        else
        {
            m_isLoaded = false;
            sEngineRef().console().print(Console::Warning, "failed!\n");
        }
    }
}

bool MusicResource::exists() const
{
    return PHYSFS_exists(m_filename.c_str());
}

bool MusicResource::isLoaded() const
{
    return m_isLoaded;
}

bool MusicResource::isPrecached() const
{
    return m_precached;
}


