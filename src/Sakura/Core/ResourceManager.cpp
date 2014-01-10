#include "Sakura/Core/ResourceManager.hpp"
#include "Sakura/Core/ResourceBase.hpp"
#include "Sakura/Core/Engine.hpp"
#include "Sakura/Resources/SoundResource.hpp"
#include "Sakura/Resources/SongResource.hpp"
#include "Sakura/Resources/TextureResource.hpp"
#include "Sakura/Resources/FontResource.hpp"
#include "Sakura/Resources/SpriteContainerResource.hpp"
#include <BinaryReader.hpp>
#include <IOException.hpp>
#include <iostream>
#include <physfs.h>

namespace Sakura
{
namespace Core
{

ResourceManager::ResourceManager()
{
    m_defaultTexture = new sf::Texture();
    sf::Image tmp;
    tmp.create(32, 32);

    sf::Color fillColor;
    for (int y = 0; y < 32; y ++)
    {
        for (int x = 0; x < 32; x++)
        {
            // if x is greater than the half width and y is still less than the half width
            // as well as if the y is greater than the half width and x is less
            // fill with red
            // otherwise fill with semi-transparent white
            if ((x > (32/2) && y < (32/2)) || (x < (32/2) && y > (32/2)))
                fillColor = sf::Color::Red;
            else
                fillColor = sf::Color(255, 255, 255, 50);

            tmp.setPixel(x, y, fillColor);
        }
    }

    m_defaultTexture->loadFromImage(tmp);
}

ResourceManager::~ResourceManager()
{
    delete m_defaultTexture;
}

bool ResourceManager::initialize(const char* argv0)
{
    sEngineRef().console().print(Console::Info, "Initializing PHYSFS");
    sEngineRef().console().print(Console::Info, "Sakura built with PHYSFS Version %i.%i.%i", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);
    PHYSFS_init(argv0);
    PHYSFS_Version version;
    PHYSFS_getLinkedVersion(&version);
    sEngineRef().console().print(Console::Info, "Local PHYSFS Version %i.%i.%i", (int)version.major, (int)version.minor, (int)version.patch);
    const PHYSFS_ArchiveInfo** archiveInfoBegin = PHYSFS_supportedArchiveTypes();

    // Print the supported archive formats
    sEngineRef().console().print(Console::Info, "Supported Archive types:");
    for (const PHYSFS_ArchiveInfo** archiveInfo = archiveInfoBegin; *archiveInfo != NULL; archiveInfo++)
    {
        if (!std::string(((PHYSFS_ArchiveInfo*)*archiveInfo)->extension).compare("ZIP"))
        {
            sEngineRef().console().print(Console::Info,    "Extension:      %s", ((PHYSFS_ArchiveInfo*)*archiveInfo)->extension);
            sEngineRef().console().print(Console::Info,    "Author:         %s", ((PHYSFS_ArchiveInfo*)*archiveInfo)->author);
            sEngineRef().console().print(Console::Info,    "Description:    %s", ((PHYSFS_ArchiveInfo*)*archiveInfo)->description);
            sEngineRef().console().print(Console::Info,    "URL:            %s", ((PHYSFS_ArchiveInfo*)*archiveInfo)->url);
            sEngineRef().console().print(Console::Message, "-------------------------");
        }
    }
    if (!PHYSFS_mount(sEngineRef().config().settingLiteral("fs_basepath", "data").c_str(), "/", 0))
    {
        sEngineRef().console().print(Console::Fatal, "Failed to mount basepath %s", sEngineRef().config().settingLiteral("fs_basepath", "data").c_str());
        return false;
    }

    char** listBegin = PHYSFS_enumerateFiles("/");
    std::vector<std::string> archives;
    for (char** file = listBegin; *file != NULL; file++)
    {
        // We don't support sub-directories for pak files
        if (PHYSFS_isDirectory(*file))
            continue;

        std::string fileName(*file);
        fileName.insert(0, std::string(PHYSFS_getRealDir(*file)) + "/");
        std::string extension = fileName.substr(fileName.find_last_of(".") + 1, fileName.length() - (fileName.find_last_of(".") + 1));
        if ((!extension.compare("zip") || !extension.compare("pak")))
            archives.push_back(fileName);
    }

    PHYSFS_freeList(listBegin);

    // Sort files in descending order
    std::sort(archives.begin(), archives.end(), std::less<std::string>());

    for (std::string archive : archives)
    {
        sEngineRef().console().print(Console::Info, "Mounting archive: %s", archive.c_str());
        PHYSFS_mount(archive.c_str(), NULL, 0);
    }

    try
    {
        zelda::io::BinaryReader reader("precache");
        sEngineRef().console().print(Console::Info, "Attempting to load precache list...");
        Uint32 magic = reader.readUInt32();
        if (magic != 0x45484353)
            throw zelda::error::IOException("Not a valid precache file");

        Uint32 fileCount = reader.readUInt32();
        while( (fileCount--) > 0)
        {
            std::string filepath = reader.readString();
            ResourceType resType = (ResourceType)reader.readByte();

            switch (resType)
            {
                case Texture:
                    loadTexture(filepath, true);
                    break;
                case Font:
                    loadFont(filepath, true);
                    break;
                case Song:
                    loadSong(filepath, true);
                    break;
                case Sound:
                    loadSound(filepath, true);
                    break;
                case SpriteContainer:
                    loadSpriteContainer(filepath, true);
                    break;
                case Map:
                    break;
                default:
                    break;
            }
        }
    }
    catch (zelda::error::Exception e)
    {
        sEngineRef().console().print(Console::Error, "Failed to load precache list:\n%s", e.message().c_str());
    }

    return true;
}

bool ResourceManager::loadSound(const std::string &name, bool preload)
{
    Resources::SoundResource* sound = new Resources::SoundResource(name, preload);
    if (sound->exists())
    {
        m_soundBufferResources[name] =  sound;
        if (sound->isPrecached())
            m_sounds[name] = new sf::Sound(*sound->data());
        return true;
    }

    delete sound;
    sound = NULL;
    return false;
}

void ResourceManager::playSound(const std::string &name)
{
    if (((Resources::SoundResource*)m_soundBufferResources[name]) != NULL)
    {
        if (!m_soundBufferResources[name]->isLoaded())
        {
            m_soundBufferResources[name]->load();
            m_sounds[name] = new sf::Sound(*m_soundBufferResources[name]->data());
        }
        m_sounds[name]->play();
        return;
    }
    else if (loadSound(name, true))
    {
        m_sounds[name]->play();
        return;
    }

    sEngineRef().console().print(Console::Warning, "Sound resource %s does not exist", name.c_str());
}

void ResourceManager::removeSound(const std::string &name)
{
    Resources::SoundResource* resource = m_soundBufferResources[name];
    sf::Sound* sound = m_sounds[name];
    m_soundBufferResources.erase(m_soundBufferResources.find(name));
    m_sounds.erase(m_sounds.find(name));
    delete sound;
    sound = NULL;
    delete resource;
    resource = NULL;
}

bool ResourceManager::soundExists(const std::string &name)
{
    std::unordered_map<std::string, Resources::SoundResource*>::const_iterator iter = m_soundBufferResources.begin();
    for (; iter != m_soundBufferResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

int ResourceManager::soundCount() const
{
    return m_soundBufferResources.size();
}

int ResourceManager::liveSoundCount() const
{
    return m_sounds.size();
}

bool ResourceManager::loadSong(const std::string& name, bool preload)
{
    Resources::SongResource* music = new Resources::SongResource(name, preload);

    if (music->exists())
    {
        m_songResources[name] = music;
        return true;
    }

    delete music;
    music = NULL;
    return false;
}

void ResourceManager::playSong(const std::string& name, bool loop)
{
    if (((Resources::SongResource*)m_songResources[name]) != NULL)
    {
        if (!m_songResources[name]->isLoaded())
        {
            m_songResources[name]->load();
        }
        m_songResources[name]->data()->setLoop(loop);
        m_songResources[name]->data()->play();
        return;
    }
    else if (loadSong(name, true))
    {
        m_songResources[name]->data()->setLoop(loop);
        m_songResources[name]->data()->play();
        return;
    }
    sEngineRef().console().print(Console::Warning, "Song %s does not exist", name.c_str());
}

void ResourceManager::removeSong(const std::string &name)
{
    Resources::SongResource* resource = m_songResources[name];
    m_songResources.erase(m_songResources.find(name));
    delete resource;
    resource = NULL;
}

bool ResourceManager::songExists(const std::string& name)
{
    std::unordered_map<std::string, Resources::SongResource*>::const_iterator iter = m_songResources.begin();
    for (; iter != m_songResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

int ResourceManager::songCount() const
{
    return m_songResources.size();
}

bool ResourceManager::loadTexture(const std::string &name, bool preload)
{
    if (textureExists(name))
        return true;

    Resources::TextureResource* texture = new Resources::TextureResource(name, preload);
    if (texture->exists())
    {
        m_textureResources[name] = texture;
        return true;
    }

    delete texture;
    texture = NULL;
    return false;
}

sf::Texture& ResourceManager::texture(const std::string &name)
{
    if (textureExists(name))
    {
        if (!m_textureResources[name]->isLoaded())
            m_textureResources[name]->load();

        return *((Resources::TextureResource*)m_textureResources[name])->data();
    }
    else if (loadTexture(name, true))
    {
        return *((Resources::TextureResource*)m_textureResources[name])->data();
    }

    sEngineRef().console().print(Console::Warning, "Texture %s does not exist", name.c_str());
    // Draw the default invalid texture.
    return *m_defaultTexture;
}

void ResourceManager::removeTexture(const std::string &name)
{
    Resources::TextureResource* resource = m_textureResources[name];
    m_textureResources.erase(m_textureResources.find(name));
    delete resource;
    resource = NULL;
}

bool ResourceManager::textureExists(const std::string &name)
{
    std::unordered_map<std::string, Resources::TextureResource*>::const_iterator iter = m_textureResources.begin();

    for (; iter != m_textureResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

int ResourceManager::textureCount() const
{
    return m_textureResources.size();
}

bool ResourceManager::loadFont(const std::string& name, bool preload)
{
    if (fontExists(name))
        return true;

    Resources::FontResource* font = new Resources::FontResource(name, preload);
    if (font->exists())
    {
        m_fontResources.insert(std::make_pair(name, font));
        return true;
    }

    delete font;
    font = NULL;
    return false;
}

sf::Font* ResourceManager::font(const std::string& name)
{
    if (fontExists(name))
    {
        if (!m_fontResources[name]->isLoaded())
            m_fontResources[name]->load();
        return ((Resources::FontResource*)m_fontResources[name])->data();
    }
    else if (loadFont(name, true))
    {
        return ((Resources::FontResource*)m_fontResources[name])->data();
    }

    sEngineRef().console().print(Console::Warning, "Font %s does not exist", name.c_str());

    return NULL;
}

void ResourceManager::removeFont(const std::string &name)
{
    Resources::FontResource* resource = m_fontResources[name];
    m_fontResources.erase(m_fontResources.find(name));
    delete resource;
    resource = NULL;
}

bool ResourceManager::fontExists(const std::string& name)
{
    std::unordered_map<std::string, Resources::FontResource*>::const_iterator iter = m_fontResources.begin();

    for (; iter != m_fontResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

int ResourceManager::fontCount() const
{
    return m_fontResources.size();
}

bool ResourceManager::loadSpriteContainer(const std::string& name, bool preload)
{
    if (spriteContainerExists(name))
        return true;

    Resources::SpriteContainerResource* sprite = new Resources::SpriteContainerResource(name, preload);
    if (sprite->exists())
    {
        m_spriteContainerResources.insert(std::make_pair(name, sprite));
        return true;
    }

    delete sprite;
    sprite = NULL;
    return false;
}

SSpriteFile* ResourceManager::spriteContainer(const std::string& name)
{
    if (spriteContainerExists(name))
    {
        if (!m_spriteContainerResources[name]->isLoaded())
            m_spriteContainerResources[name]->load();
        return ((Resources::SpriteContainerResource*)m_spriteContainerResources[name])->data();
    }
    else if (loadSpriteContainer(name, true))
    {
        return ((Resources::SpriteContainerResource*)m_spriteContainerResources[name])->data();
    }

    sEngineRef().console().print(Console::Warning, "Sprite container %s does not exist", name.c_str());

    return NULL;
}

void ResourceManager::removeSpriteContainer(const std::string& name)
{
    Resources::SpriteContainerResource* resource = m_spriteContainerResources[name];
    m_spriteContainerResources.erase(m_spriteContainerResources.find(name));
    delete resource;
    resource = NULL;
}

bool ResourceManager::spriteContainerExists(const std::string& name) const
{
    std::unordered_map<std::string, Resources::SpriteContainerResource*>::const_iterator iter = m_spriteContainerResources.begin();

    for (; iter != m_spriteContainerResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

int ResourceManager::spriteContainerCount() const
{
    return m_spriteContainerResources.size();
}

void ResourceManager::shutdown()
{
    purgeResources();
    sEngineRef().console().print(Console::Info, "Shutting down PHYSFS...");
    PHYSFS_deinit();
}

void ResourceManager::purgeResources()
{
    sEngineRef().console().print(Console::Info, "Purging resources...");
    std::unordered_map<std::string, Resources::TextureResource*>::iterator textureIter = m_textureResources.begin();
    for (; textureIter != m_textureResources.end(); ++textureIter)
    {
        delete textureIter->second;
        textureIter->second = NULL;
    }
    m_textureResources.clear();

    std::unordered_map<std::string, sf::Sound*>::iterator soundIter = m_sounds.begin();

    for (; soundIter != m_sounds.end(); ++soundIter)
    {
        delete soundIter->second;
        soundIter->second = NULL;
    }

    m_sounds.clear();


    std::unordered_map<std::string, Resources::SoundResource*>::iterator soundBufferIter = m_soundBufferResources.begin();


    for (; soundBufferIter != m_soundBufferResources.end(); ++soundBufferIter)
    {
        delete soundBufferIter->second;
        soundBufferIter->second = NULL;
    }

    m_soundBufferResources.clear();

    std::unordered_map<std::string, Resources::FontResource*>::iterator fontIter = m_fontResources.begin();

    for (; fontIter != m_fontResources.end(); ++fontIter)
    {
        delete fontIter->second;
        fontIter->second = NULL;
    }

    m_fontResources.clear();

    std::unordered_map<std::string, Resources::SongResource*>::iterator musicIter = m_songResources.begin();
    for (; musicIter != m_songResources.end(); ++musicIter)
    {
        delete musicIter->second;
        musicIter->second = NULL;
    }

    m_songResources.clear();

    std::unordered_map<std::string, Resources::SpriteContainerResource*>::iterator spriteIter = m_spriteContainerResources.begin();
    for (; spriteIter != m_spriteContainerResources.end(); ++spriteIter)
    {
        delete spriteIter->second;
        spriteIter->second = NULL;
    }

    m_spriteContainerResources.clear();
}

} // Core
} // Sakura

