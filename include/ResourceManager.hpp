#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <string>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

class SoundResource;
class MusicResource;
class TextureResource;
class FontResource;

class ResourceManager
{
public:
    static const std::string BASEDIR;
    static const std::string SPRITEDIR;
    static const std::string BACKGROUNDDIR;
    static const std::string MAPDIR;
    static const std::string SOUNDDIR;
    static const std::string MUSICDIR;
    static const std::string FONTDIR;

    ~ResourceManager();
    void initialize(const char* argv0);
    bool loadSound(const std::string& name, SoundResource* sound);
    void playSound(const std::string& name);
    void removeSound(const std::string& name);
    bool soundExists(const std::string& name);
    int soundCount() const;
    int liveSoundCount() const;

    bool loadMusic(const std::string& name, MusicResource* sound);
    void playMusic(const std::string& name);
    void removeMusic(const std::string& name);
    bool musicExists(const std::string& name);
    int musicCount() const;

    bool loadTexture(const std::string& name, TextureResource* texture);
    sf::Texture* texture(const std::string& name);
    void removeTexture(const std::string& name);
    bool textureExists(const std::string& name);
    int textureCount() const;

    bool loadFont(const std::string& name, FontResource* font);
    sf::Font* font(const std::string& name);
    void removeFont(const std::string& name);
    bool fontExists(const std::string& name);
    int fontCount() const;
    void shutdown();
    void purgeResources();
private:

    std::unordered_map<std::string, SoundResource*>   m_soundBufferResources;
    std::unordered_map<std::string, sf::Sound*>       m_sounds;
    std::unordered_map<std::string, MusicResource*>   m_musicResources;
    std::unordered_map<std::string, TextureResource*> m_textureResources;
    std::unordered_map<std::string, FontResource*>    m_fontResources;
};

#endif // RESOURCEMANAGER_HPP
