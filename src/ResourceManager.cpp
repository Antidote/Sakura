#include "ResourceManager.hpp"
#include "Resource.hpp"
#include "SoundResource.hpp"
#include "MusicResource.hpp"
#include "TextureResource.hpp"
#include "FontResource.hpp"
#include "Engine.hpp"
#include <iostream>
#include <physfs.h>

const std::string ResourceManager::BASEDIR = std::string("data");
const std::string ResourceManager::SPRITEDIR = std::string("sprites/");
const std::string ResourceManager::BACKGROUNDDIR = std::string("backgrounds/");
const std::string ResourceManager::MAPDIR = std::string("maps/");
const std::string ResourceManager::SOUNDDIR = std::string("sounds/");
const std::string ResourceManager::MUSICDIR = std::string("music/");
const std::string ResourceManager::FONTDIR = std::string("fonts/");

ResourceManager::~ResourceManager()
{
}

void ResourceManager::initialize(const char* argv0)
{
    PHYSFS_init(argv0);
    PHYSFS_mount("data", "/", 0);

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
        Engine::instance().console().print(Console::Info, "Mounting archive: %s", archive.c_str());
        PHYSFS_mount(archive.c_str(), NULL, 1);
    }
}

bool ResourceManager::loadSound(const std::string &name, SoundResource *sound)
{
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
    }
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

bool ResourceManager::loadMusic(const std::string& name, MusicResource* music)
{
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

bool ResourceManager::loadTexture(const std::string &name, TextureResource *texture)
{
    if (texture->exists())
    {
        m_textureResources[name] = texture;
        return true;
    }

    delete texture;
    texture = NULL;
    return false;
}

sf::Texture* ResourceManager::texture(const std::string &name)
{
    if (textureExists(name))
    {
        if (!m_textureResources[name]->isLoaded())
            m_textureResources[name]->load();

        return ((TextureResource*)m_textureResources[name])->data();
    }

    return NULL;
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

bool ResourceManager::loadFont(const std::string& name, FontResource* font)
{
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
    Engine::instance().console().print(Console::Info, "Shutting down PHYSFS...");
    PHYSFS_deinit();
    purgeResources();
}

void ResourceManager::purgeResources()
{
    Engine::instance().console().print(Console::Info, "Purging resources...");
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

