#include "ResourceManager.hpp"
#include "Resource.hpp"
#include "SoundResource.hpp"
#include "TextureResource.hpp"
#include <iostream>


ResourceManager::~ResourceManager()
{
    purgeResources();
}

bool ResourceManager::loadSound(const std::string &name, SoundResource *sound)
{
    if (sound->isLoaded())
    {
        m_soundResources.insert(std::make_pair(name, sound));
        return true;
    }

    delete sound;
    sound = NULL;
    return false;
}

sf::Sound* ResourceManager::sound(const std::string &name)
{
    if (((SoundResource*)m_soundResources[name]) != NULL)
    {
        sf::Sound* sound = new sf::Sound;
        sound->setBuffer(*((SoundResource*)m_soundResources[name])->data());
        return sound;
    }

    return NULL;
}

bool ResourceManager::soundExists(const std::string &name)
{
    std::unordered_map<std::string, SoundResource*>::const_iterator iter = m_soundResources.begin();
    for (; iter != m_soundResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

bool ResourceManager::loadTexture(const std::string &name, TextureResource *texture)
{
    if (texture->isLoaded())
    {
        m_textureResources.insert(std::make_pair(name, texture));
        return true;
    }

    delete texture;
    texture = NULL;
    return false;
}

sf::Texture* ResourceManager::texture(const std::string &name)
{
    if (textureExists(name))
        return ((TextureResource*)m_textureResources[name])->data();

    return NULL;
}

bool ResourceManager::textureExists(const std::string &name)
{
    std::unordered_map<std::string, TextureResource*>::const_iterator iter = m_textureResources.begin();

    for (; iter != m_textureResources.end(); ++iter)
        if (iter->first == name)
            return true;

    return false;
}

void ResourceManager::purgeResources()
{
    std::unordered_map<std::string, TextureResource*>::iterator textureIter = m_textureResources.begin();
    for (; textureIter != m_textureResources.end(); ++textureIter)
    {
        delete textureIter->second;
        textureIter->second = NULL;
    }
    m_textureResources.clear();

    std::unordered_map<std::string, SoundResource*>::iterator soundIter = m_soundResources.begin();

    for (; soundIter != m_soundResources.end(); ++soundIter)
    {
        delete soundIter->second;
        soundIter->second = NULL;
    }

    m_soundResources.clear();

}

