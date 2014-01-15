#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/ResourceManager.hpp"
#include "Sakura/Resources/FontResource.hpp"
#include <physfs.h>
#include <iostream>

namespace Sakura
{
namespace Resources
{
FontResource::FontResource(const std::string& filepath, bool precache)
    : ResourceBase(filepath, precache)
{
    if (precache && exists())
    {
        sEngineRef().console().print(Core::Console::Info, "Precaching font %s@%s...", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
        load();
    }
}

FontResource::~FontResource()
{
    if (exists())
        sEngineRef().console().print(Core::Console::Info, "Font destroyed %s@%s", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());

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
    if (!exists())
        return;

    if (!m_precached)
        sEngineRef().console().print(Core::Console::Info, "Loading font %s@%s...", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());

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
                sEngineRef().console().print(Core::Console::Message, "done.");
            }
            else
            {
                m_isLoaded = false;
                sEngineRef().console().print(Core::Console::Warning, "failed!");
            }
        }

        PHYSFS_close(file);
        return;
    }
    sEngineRef().console().print(Core::Console::Warning, "Font %s@%s does not exist", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
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

} // Resources
} // Sakura
