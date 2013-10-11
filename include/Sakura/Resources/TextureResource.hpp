#ifndef TEXTURERESOURCE_HPP
#define TEXTURERESOURCE_HPP

#include "Sakura/Core/ResourceBase.hpp"
#include <SFML/Graphics/Texture.hpp>

namespace Sakura
{
namespace Resources
{
class TextureResource : public Core::ResourceBase<sf::Texture>
{
public:
    /*!
     * \brief TextureResource
     * \param filename
     * \param precache
     */
    TextureResource(const std::string& filename, bool precache = false);
    ~TextureResource();

    /*!
     * \brief filename
     * \return
     */
    std::string filename() const;

    /*!
     * \brief data
     * \return
     */
    sf::Texture* data();

    /*!
     * \brief load
     */
    void load();

    /*!
     * \brief exists
     * \return
     */
    bool exists() const;

    /*!
     * \brief isLoaded
     * \return
     */
    bool isLoaded() const;

    /*!
     * \brief isPrecached
     * \return
     */
    bool isPrecached() const;
private:
    void setData(sf::Texture *data);
};
} // Resources
} // Sakura


#endif // TEXTURERESOURCE_HPP
