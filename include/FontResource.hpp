#ifndef FONTRESOURCE_HPP
#define FONTRESOURCE_HPP

#include <SFML/Graphics/Font.hpp>
#include "Resource.hpp"

class FontResource : public Resource<sf::Font>
{
public:
    FontResource(const std::string& filepath, bool precache = false);
    ~FontResource();

    std::string filename() const;
    sf::Font* data();

    void load();
    bool exists() const;
    bool isLoaded() const;
    bool isPrecached() const;

private:
    void setData(sf::Font *data);
};

#endif // FONTRESOURCE_HPP
