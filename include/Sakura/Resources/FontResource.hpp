#ifndef FONTRESOURCE_HPP
#define FONTRESOURCE_HPP

#include <SFML/Graphics/Font.hpp>
#include "Sakura/Core/ResourceBase.hpp"

namespace Sakura
{
namespace Resources
{
class FontResource : public Core::ResourceBase<sf::Font>
{
public:
    /*!
     * \brief FontResource
     * \param filepath
     * \param precache
     */
    FontResource(const std::string& filepath, bool precache = false);
    ~FontResource();

    /*!
     * \brief filename
     * \return
     */
    std::string filename() const;
    sf::Font* data();

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
    void setData(sf::Font *data);
};
} // Resources
} // Sakura

#endif // FONTRESOURCE_HPP
