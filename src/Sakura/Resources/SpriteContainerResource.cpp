#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/SSpriteFileReader.hpp"
#include "Sakura/Resources/SpriteContainerResource.hpp"
#include <physfs.h>
#include <Exception.hpp>

namespace Sakura
{
namespace Resources
{
SpriteContainerResource::SpriteContainerResource(const std::string &filename, bool precache)
    : ResourceBase(filename, precache)
{
    if (precache && exists())
    {
        sEngineRef().console().print(Core::Console::Info, "Precaching sprite container %s@%s", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());
        load();
    }
}

SpriteContainerResource::~SpriteContainerResource()
{
    if (exists())
        sEngineRef().console().print(Core::Console::Info, "Sprite container destroyed %s@%s", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());

    delete m_data;
}

std::string SpriteContainerResource::filename() const
{
    return m_filename;
}

Core::SSpriteFile*SpriteContainerResource::data()
{
    return m_data;
}

void SpriteContainerResource::load()
{
    if (!exists())
        return;

    if (!m_precached)
        sEngineRef().console().print(Core::Console::Info, "Loading sprite container %s@%s...", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());

    PHYSFS_file* file = PHYSFS_openRead(m_filename.c_str());
    if (file)
    {
        sf::Uint8* data = new sf::Uint8[PHYSFS_fileLength(file)];
        sf::Uint32 readLen = PHYSFS_read(file, data, 1, PHYSFS_fileLength(file));

        if (readLen == PHYSFS_fileLength(file))
        {
            try
            {
                Core::SSpriteFileReader reader(data, readLen);
                m_data = reader.readFile();
                m_isLoaded = true;
                sEngineRef().console().print(Core::Console::Message, "done.");
            }
            catch (zelda::error::Exception e)
            {
                m_isLoaded = false;
                sEngineRef().console().print(Core::Console::Warning, "%s failed!", e.message().c_str());
            }
        }

        PHYSFS_close(file);
    }
    else
    {
        m_isLoaded = false;
        sEngineRef().console().print(Core::Console::Warning, "Sprite container %s@%s does not exist", PHYSFS_getRealDir(m_filename.c_str()), m_filename.c_str());

    }
}

bool SpriteContainerResource::exists() const
{
    return PHYSFS_exists(m_filename.c_str());
}

bool SpriteContainerResource::isLoaded() const
{
    return m_isLoaded;
}

bool SpriteContainerResource::isPrecached()
{
    return m_precached;
}

}
}
