#include "FontResource.hpp"
#include "ResourceManager.hpp"
#include "Engine.hpp"
#include "Log.hpp"
#include <physfs.h>
#include <iostream>


FontResource::FontResource(const std::string& filepath, bool precache)
    : Resource(ResourceManager::FONTDIR + filepath, precache)
{
    if (precache)
    {
        Engine::instance().log().print(Log::Info, "Precaching font %s...", m_filename.c_str());
        load();
    }
}

FontResource::~FontResource()
{
    Engine::instance().log().print(Log::Info, "Font destroyed %s\n", m_filename.c_str());
    delete m_data;
}

std::string FontResource::filename() const
{
    return m_filename;
}

sf::Font* FontResource::data()
{
    return m_data;
}

void FontResource::load()
{
    if (!m_precached)
        Engine::instance().log().print(Log::Info, "Loading font %s...", m_filename.c_str());

    PHYSFS_file* file = PHYSFS_openRead(m_filename.c_str());
    if (file)
    {
        sf::Uint8* data = new sf::Uint8[PHYSFS_fileLength(file)];
        sf::Uint32 readLen = PHYSFS_read(file, data, 1, PHYSFS_fileLength(file));

        if (readLen == PHYSFS_fileLength(file))
        {
            m_data = new sf::Font;
            if (m_data->loadFromMemory(data, PHYSFS_fileLength(file)))
            {
                m_isLoaded = true;
                Engine::instance().log().print(Log::Message, "done.\n");
            }
            else
            {
                m_isLoaded = false;
                Engine::instance().log().print(Log::Warning, "failed!\n");
            }
        }

        PHYSFS_close(file);
    }
}

bool FontResource::exists() const
{
    return PHYSFS_exists(m_filename.c_str());
}

bool FontResource::isLoaded() const
{
    return m_isLoaded;
}

bool FontResource::isPrecached() const
{
    return m_precached;
}

void FontResource::setData(sf::Font* data)
{
    m_data = data;
}
