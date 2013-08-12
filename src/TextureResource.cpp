#include "TextureResource.hpp"
#include <iostream>
#include <physfs.h>
#include "Engine.hpp"
#include "Log.hpp"

TextureResource::TextureResource(const std::string &filename, bool precache)
    : Resource(filename, precache)
{
    if (precache)
    {
        Engine::instance().log().print(Log::Info, "Precaching texture %s...", m_filename.c_str());
        load();
    }
}

TextureResource::~TextureResource()
{
    Engine::instance().log().print(Log::Info, "Texture destroyed %s\n", m_filename.c_str());
    delete m_data;
}

std::string TextureResource::filename() const
{
    return m_filename;
}

sf::Texture* TextureResource::data()
{
    return m_data;
}

void TextureResource::load()
{
    if (!m_precached)
        Engine::instance().log().print(Log::Info, "Loading texture %s...", m_filename.c_str());
    PHYSFS_file* file = PHYSFS_openRead(m_filename.c_str());
    if (file)
    {
        sf::Uint8* data = new sf::Uint8[PHYSFS_fileLength(file)];
        sf::Uint32 readLen = PHYSFS_read(file, data, 1, PHYSFS_fileLength(file));

        if (readLen == PHYSFS_fileLength(file))
        {
            m_data = new sf::Texture;
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

        delete[] data;
        data = NULL;
        PHYSFS_close(file);
    }
    else
    {

        // Push it to a new line
        Engine::instance().log().print(Log::Message, "\n");
        Engine::instance().log().print(Log::Warning, "Texture %s does not exist\n", m_filename.c_str());
    }
}

bool TextureResource::exists() const
{
    return PHYSFS_exists(m_filename.c_str());
}

bool TextureResource::isLoaded() const
{
    return m_isLoaded;
}

bool TextureResource::isPrecached() const
{
    return m_precached;
}

void TextureResource::setData(sf::Texture *data)
{
    m_data = data;
}
