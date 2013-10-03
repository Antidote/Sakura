#include "SoundResource.hpp"
#include "Engine.hpp"
#include <iostream>
#include <physfs.h>

SoundResource::SoundResource(const std::string &filename, bool precache)
    : Resource(filename, precache)
{
    if (precache)
    {
        sEngineRef().console().print(Console::Info, "Precaching sound %s...", m_filename.c_str());
        load();
    }
}

SoundResource::~SoundResource()
{
    sEngineRef().console().print(Console::Info, "Sound destroyed %s...", m_filename.c_str());
    delete m_data;
}

sf::SoundBuffer* SoundResource::data() const
{
    return m_data;
}

std::string SoundResource::filename() const
{
    return m_filename;
}

bool SoundResource::isLoaded() const
{
    return m_isLoaded;
}

bool SoundResource::exists() const
{
    return PHYSFS_exists(m_filename.c_str());
}

bool SoundResource::isPrecached()
{
    return m_precached;
}

void SoundResource::load()
{
    if (!m_precached)
        sEngineRef().console().print(Console::Info, "Loading sound %s...", m_filename.c_str());

    PHYSFS_file* file = PHYSFS_openRead(m_filename.c_str());
    if (file)
    {
        sf::Uint8* data = new sf::Uint8[PHYSFS_fileLength(file)];
        sf::Uint32 readLen = PHYSFS_read(file, data, 1, PHYSFS_fileLength(file));

        if (readLen == PHYSFS_fileLength(file))
        {
            m_data = new sf::SoundBuffer;
            if (m_data->loadFromMemory(data, PHYSFS_fileLength(file)))
            {
                m_isLoaded = true;
                sEngineRef().console().print(Console::Message, "done.");
            }
            else
            {
                m_isLoaded = false;
                sEngineRef().console().print(Console::Warning, "failed!");
            }
        }

        delete[] data;
        data = NULL;
        PHYSFS_close(file);
    }
}

void SoundResource::setData(sf::SoundBuffer *data)
{
    m_data = data;
}


