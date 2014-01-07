#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <string>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

namespace Sakura
{
namespace Resources
{
class SoundResource;
class SongResource;
class TextureResource;
class FontResource;
class SpriteResource;
}

namespace Core
{
class SSpriteFile;
/*!
 * \class ResourceManager
 * \brief Manages a list of all resources currently loaded, it's been designed to load resources dynamically.
 */
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
    bool initialize(const char* argv0);

    /*!
     * \brief Loads the specified sound if it exists in the filesystem.
     * \param name The path to the sound
     * \param preload Whether to load the file immediately, useful if you intend to use the resource right away.
     * \return True if sound was loaded successfully, false otherwise.
     */
    bool loadSound(const std::string& name, bool preload = false);

    /*!
     * \brief playSound Plays a sound, automatically loading it if it does not exist.
     * \param name The sound to play
     */
    void playSound(const std::string& name);

    /*!
     * \brief Removes the specified sound if it exists.
     * \param name The sound to remove.
     */
    void removeSound(const std::string& name);

    /*!
     * \brief Returns whether or not a sound exists in the resource manager
     * \param name The sound to check
     * \return True if the sound Exists, false otherwise
     */
    bool soundExists(const std::string& name);

    /*!
     * \brief Returns the amount of sounds in the resource manager
     * \return The number of sounds
     */
    int soundCount() const;

    /*!
     * \brief Returns the amount of loaded sounds that have been used in the resource manager
     * \return The number of loaded sounds that have been used
     */
    int liveSoundCount() const;

    /*!
     * \brief Loads the specified song if it exists in the filesystem.
     * \param name The path to the song
     * \param preload Whether to load the file immediately, useful if you intend to use the resource right away.
     * \return True if song was loaded successfully, false otherwise.
     */
    bool loadSong(const std::string& name, bool preload = false);

    /*!
     * \brief playSound Plays a song, automatically loading it if it does not exist.
     * \param name The song to play
     * \param loop Whether to loop the song
     */
    void playSong(const std::string& name, bool loop = false);

    /*!
     * \brief removeMusic
     * \param name
     */
    void removeSong(const std::string& name);

    /*!
     * \brief Returns whether or not a song exists in the resource manager
     * \param name The sound to check
     * \return True if the song Exists, false otherwise
     */
    bool songExists(const std::string& name);

    /*!
     * \brief Returns the amount of songs in the resource manager
     * \return The number of songs
     */
    int songCount() const;

    /*!
     * \brief Loads the specified texture if it exists in the filesystem.
     * \param name The path to the texture
     * \param preload Whether to load the file immediately, useful if you intend to use the resource right away.
     * \return True if texture was loaded successfully, false otherwise.
     */
    bool loadTexture(const std::string& name, bool preload = false);

    /*!
     * \brief Returns the specified texture if it exists, it will also attempt to load it if it's unavailable.
     * \param name The texture to return.
     * \return The texture if it exists, NULL otherwise
     */
    sf::Texture& texture(const std::string& name);

    /*!
     * \brief Removes the specified texture if it exists.
     * \param name The texture to remove.
     */
    void removeTexture(const std::string& name);

    /*!
     * \brief Returns whether or not a texture exists in the resource manager
     * \param name The texture to check
     * \return True if the texture Exists, false otherwise
     */
    bool textureExists(const std::string& name);

    /*!
     * \brief Returns the amount of textures in the resource manager
     * \return The number of textures
     */
    int textureCount() const;

    /*!
     * \brief Loads the specified font if it exists in the filesystem.
     * \param name The path to the font
     * \param preload Whether to load the file immediately, useful if you intend to use the resource right away.
     * \return True if font was loaded successfully, false otherwise.
     */
    bool loadFont(const std::string& name, bool preload = false);

    /*!
     * \brief Returns the specified font if it exists, it will also attempt to load it if it's unavailable.
     * \param name The font to return.
     * \return The font if it exists, NULL otherwise
     */
    sf::Font* font(const std::string& name);

    /*!
     * \brief Removes the specified font if it exists.
     * \param name The font to remove.
     */
    void removeFont(const std::string& name);

    /*!
     * \brief Returns whether or not a font exists in the resource manager
     * \param name The font to check
     * \return True if the font Exists, false otherwise
     */
    bool fontExists(const std::string& name);

    /*!
     * \brief Returns the amount of fonts in the resource manager
     * \return The number of fonts
     */
    int fontCount() const;

    bool loadSprite(const std::string& name, bool preload = false);
    SSpriteFile* sprite(const std::string& name);
    void removeSprite(const std::string& name);
    bool spriteExists(const std::string& name) const;
    int spriteCount() const;

    /*!
     * \brief Shuts down the ResourceManager
     */
    void shutdown();

    /*!
     * \brief Removes all Resources.
     */
    void purgeResources();

private:
    sf::Texture*                                      m_defaultTexture;

    std::unordered_map<std::string, Resources::SoundResource*>   m_soundBufferResources;
    std::unordered_map<std::string, sf::Sound*>                  m_sounds;
    std::unordered_map<std::string, Resources::SongResource*>    m_songResources;
    std::unordered_map<std::string, Resources::TextureResource*> m_textureResources;
    std::unordered_map<std::string, Resources::FontResource*>    m_fontResources;
    std::unordered_map<std::string, Resources::SpriteResource*>  m_spriteResources;
};

} // Core
} // Sakura

#endif // RESOURCEMANAGER_HPP
