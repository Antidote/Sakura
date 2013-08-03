#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <string>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>

class SoundResource;
class TextureResource;

class ResourceManager
{
public:
    ~ResourceManager();

    bool loadSound(const std::string& name, SoundResource* sound);
    sf::Sound* sound(const std::string& name);
    bool soundExists(const std::string& name);

    bool loadTexture(const std::string& name, TextureResource* texture);
    sf::Texture* texture(const std::string& name);
    bool textureExists(const std::string& name);

    void purgeResources();
private:

    std::unordered_map<std::string, SoundResource*> m_soundResources;
    std::unordered_map<std::string, TextureResource*> m_textureResources;
};

#endif // RESOURCEMANAGER_HPP
