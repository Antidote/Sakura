#ifndef FONTRESOURCE_HPP
#define FONTRESOURCE_HPP

#include <SFML/Graphics/Font.hpp>
#include "Resource.hpp"

class FontResource : public Resource<sf::Font>
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

#endif // FONTRESOURCE_HPP
