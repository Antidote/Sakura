#include "Sakura/Core/Engine.hpp"
#include "Sakura/Resources/TextureResource.hpp"
#include <iostream>
#include <physfs.h>

namespace Sakura
{
namespace Resources
{
TextureResource::TextureResource(const std::string &filename, bool precache)
    : ResourceBase(filename, precache)
{
    if (precache && exists())
    {
        sEngineRef().console().print(Core::Console::Info, "Precaching texture %s@%s...", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
        load();
    }
}

TextureResource::~TextureResource()
{
    if (exists())
        sEngineRef().console().print(Core::Console::Info, "Texture destroyed %s@%s", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
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
    if (!exists())
         return;

    if (!m_precached)
        sEngineRef().console().print(Core::Console::Info, "Loading texture %s@%s...", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());

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
                sEngineRef().console().print(Core::Console::Message, "done.");
            }
            else
            {
                m_isLoaded = false;
                sEngineRef().console().print(Core::Console::Warning, "failed!");
            }
        }

        delete[] data;
        data = NULL;
        PHYSFS_close(file);
    }
    else
    {
        sEngineRef().console().print(Core::Console::Warning, "Texture %s@%s does not exist", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
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

}
}

