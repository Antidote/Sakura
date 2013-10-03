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
    /*!
     * \brief ResourceManager
     */
    ResourceManager();
    ~ResourceManager();

    /*!
     * \brief initialize Initializes the resource subsystem, starting PHYSFS, and mounting any archives (.pak or .zip) available
     * \param argv0 Used by PHYSFS to determine the current working directory.
     */
    void initialize(const char* argv0);

    /*!
     * \brief loadSound
     * \param name
     * \param preload
     * \return
     */
    bool loadSound(const std::string& name, bool preload = false);

    /*!
     * \brief playSound Plays a sound, automatically loading it if it does not exist.
     * \param name
     */
    void playSound(const std::string& name);

    /*!
     * \brief removeSound
     * \param name
     */
    void removeSound(const std::string& name);

    /*!
     * \brief soundExists
     * \param name
     * \return
     */
    bool soundExists(const std::string& name);

    /*!
     * \brief soundCount
     * \return
     */
    int soundCount() const;

    /*!
     * \brief liveSoundCount
     * \return
     */
    int liveSoundCount() const;

    /*!
     * \brief loadMusic
     * \param name
     * \param preload
     * \return
     */
    bool loadMusic(const std::string& name, bool preload = false);

    /*!
     * \brief playMusic
     * \param name
     */
    void playMusic(const std::string& name);

    /*!
     * \brief removeMusic
     * \param name
     */
    void removeMusic(const std::string& name);

    /*!
     * \brief musicExists
     * \param name
     * \return
     */
    bool musicExists(const std::string& name);

    /*!
     * \brief musicCount
     * \return
     */
    int musicCount() const;

    /*!
     * \brief loadTexture
     * \param name
     * \param preload
     * \return
     */
    bool loadTexture(const std::string& name, bool preload = false);

    /*!
     * \brief texture
     * \param name
     * \return
     */
    sf::Texture& texture(const std::string& name);

    /*!
     * \brief removeTexture
     * \param name
     */
    void removeTexture(const std::string& name);

    /*!
     * \brief textureExists
     * \param name
     * \return
     */
    bool textureExists(const std::string& name);

    /*!
     * \brief textureCount
     * \return
     */
    int textureCount() const;

    /*!
     * \brief loadFont
     * \param name
     * \param preload
     * \return
     */
    bool loadFont(const std::string& name, bool preload = false);

    /*!
     * \brief font
     * \param name
     * \return
     */
    sf::Font* font(const std::string& name);

    /*!
     * \brief removeFont
     * \param name
     */
    void removeFont(const std::string& name);

    /*!
     * \brief fontExists
     * \param name
     * \return
     */
    bool fontExists(const std::string& name);

    /*!
     * \brief fontCount
     * \return
     */
    int fontCount() const;

    /*!
     * \brief shutdown
     */
    void shutdown();

    /*!
     * \brief purgeResources
     */
    void purgeResources();

private:
    sf::Texture*                                      m_defaultTexture;

    std::unordered_map<std::string, SoundResource*>   m_soundBufferResources;
    std::unordered_map<std::string, sf::Sound*>       m_sounds;
    std::unordered_map<std::string, MusicResource*>   m_musicResources;
    std::unordered_map<std::string, TextureResource*> m_textureResources;
    std::unordered_map<std::string, FontResource*>    m_fontResources;
};

#endif // RESOURCEMANAGER_HPP
