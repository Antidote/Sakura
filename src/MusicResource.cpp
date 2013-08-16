#include "MusicResource.hpp"
#include "ResourceManager.hpp"
#include "Engine.hpp"
#include <physfs.h>



MusicResource::MusicResource(const std::string& filename, bool precache)
    : Resource(ResourceManager::MUSICDIR + filename, precache)
{
    if (precache)
    {
        Engine::instance().console().print(Console::Info, "Precaching song %s...", m_filename.c_str());
        load();
    }
}

MusicResource::~MusicResource()
{
    Engine::instance().console().print(Console::Info, "Song destroyed %s\n", m_filename.c_str());
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
        Engine::instance().console().print(Console::Info, "Loading song %s...", m_filename.c_str());

    PHYSFS_file* file = PHYSFS_openRead(m_filename.c_str());
    if (file)
    {
        sf::Uint8* data = new sf::Uint8[PHYSFS_fileLength(file)];
        sf::Uint32 readLen = PHYSFS_read(file, data, 1, PHYSFS_fileLength(file));

        if (readLen == PHYSFS_fileLength(file))
        {
            m_data = new sf::Music;
            if (m_data->openFromMemory(data, PHYSFS_fileLength(file)))
            {
                m_isLoaded = true;
                Engine::instance().console().print(Console::Message, "done.");
            }
            else
            {
                m_isLoaded = false;
                Engine::instance().console().print(Console::Warning, "failed!\n");
            }
        }

        PHYSFS_close(file);
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


