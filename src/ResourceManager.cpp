#include "ResourceManager.hpp"
#include "Resource.hpp"
#include "SoundResource.hpp"
#include "MusicResource.hpp"
#include "TextureResource.hpp"
#include "FontResource.hpp"
#include "Engine.hpp"
#include <iostream>
#include <physfs.h>

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
            // otherwise fill with transparent
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

void ResourceManager::initialize(const char* argv0)
{
    PHYSFS_init(argv0);
    PHYSFS_mount(sEngineRef().config().settingLiteral("fs_basepath", "data").c_str(), "/", 0);

    char** listBegin = PHYSFS_enumerateFiles("/");
    std::vector<std::string> archives;
    for (char** file = listBegin; *file != NULL; file++)
    {
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
    std::sort(archives.begin(), archives.end(), std::greater<std::string>());

    for (std::string archive : archives)
    {
        sEngineRef().console().print(Console::Info, "Mounting archive: %s", archive.c_str());
        PHYSFS_mount(archive.c_str(), NULL, 1);
    }
}

bool ResourceManager::loadSound(const std::string &name, bool preload)
{
    SoundResource* sound = new SoundResource(name, preload);
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
    if (((SoundResource*)m_soundBufferResources[name]) != NULL)
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
    SoundResource* resource = m_soundBufferResources[name];
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
    std::unordered_map<std::string, SoundResource*>::const_iterator iter = m_soundBufferResources.begin();
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

bool ResourceManager::loadMusic(const std::string& name, bool preload)
{
    if (musicExists(name))
        return true;

    MusicResource* music = new MusicResource(name, preload);

    if (music->exists())
    {
        m_musicResources[name] = music;
        return true;
    }

    delete music;
    music = NULL;
    return false;
}

void ResourceManager::playMusic(const std::string& name)
{
    if (((MusicResource*)m_musicResources[name]) != NULL)
    {
        if (!m_musicResources[name]->isLoaded())
        {
            m_musicResources[name]->load();
        }
        m_musicResources[name]->data()->play();
    }
}

void ResourceManager::removeMusic(const std::string &name)
{
    MusicResource* resource = m_musicResources[name];
    m_musicResources.erase(m_musicResources.find(name));
    delete resource;
    resource = NULL;
}

bool ResourceManager::musicExists(const std::string& name)
{
    std::unordered_map<std::string, MusicResource*>::const_iterator iter = m_musicResources.begin();
    for (; iter != m_musicResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

int ResourceManager::musicCount() const
{
    return m_musicResources.size();
}

bool ResourceManager::loadTexture(const std::string &name, bool preload)
{
    if (textureExists(name))
        return true;

    TextureResource* texture = new TextureResource(name, preload);
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

        return *((TextureResource*)m_textureResources[name])->data();
    }
    else if (loadTexture(name, true))
    {
        return *((TextureResource*)m_textureResources[name])->data();
    }

    sEngineRef().console().print(Console::Warning, "Texture %s does not exist", name.c_str());
    // Draw the default invalid texture.
    return *m_defaultTexture;
}

void ResourceManager::removeTexture(const std::string &name)
{
    TextureResource* resource = m_textureResources[name];
    m_textureResources.erase(m_textureResources.find(name));
    delete resource;
    resource = NULL;
}

bool ResourceManager::textureExists(const std::string &name)
{
    std::unordered_map<std::string, TextureResource*>::const_iterator iter = m_textureResources.begin();

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

    FontResource* font = new FontResource(name, preload);
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
        return ((FontResource*)m_fontResources[name])->data();
    }
    else if (loadFont(name, true))
    {
        return ((FontResource*)m_fontResources[name])->data();
    }

    return NULL;
}

void ResourceManager::removeFont(const std::string &name)
{
    FontResource* resource = m_fontResources[name];
    m_fontResources.erase(m_fontResources.find(name));
    delete resource;
    resource = NULL;
}

bool ResourceManager::fontExists(const std::string& name)
{
    std::unordered_map<std::string, FontResource*>::const_iterator iter = m_fontResources.begin();

    for (; iter != m_fontResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

int ResourceManager::fontCount() const
{
    return m_fontResources.size();
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
    std::unordered_map<std::string, TextureResource*>::iterator textureIter = m_textureResources.begin();
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


    std::unordered_map<std::string, SoundResource*>::iterator soundBufferIter = m_soundBufferResources.begin();


    for (; soundBufferIter != m_soundBufferResources.end(); ++soundBufferIter)
    {
        delete soundBufferIter->second;
        soundBufferIter->second = NULL;
    }

    m_soundBufferResources.clear();

    std::unordered_map<std::string, FontResource*>::iterator fontIter = m_fontResources.begin();

    for (; fontIter != m_fontResources.end(); ++fontIter)
    {
        delete fontIter->second;
        fontIter->second = NULL;
    }

    m_fontResources.clear();

    std::unordered_map<std::string, MusicResource*>::iterator musicIter = m_musicResources.begin();
    for (; musicIter != m_musicResources.end(); ++musicIter)
    {
        delete musicIter->second;
        musicIter->second = NULL;
    }

    m_musicResources.clear();
}

