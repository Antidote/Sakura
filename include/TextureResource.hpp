#ifndef TEXTURERESOURCE_HPP
#define TEXTURERESOURCE_HPP

#include "Resource.hpp"
#include <SFML/Graphics/Texture.hpp>

class TextureResource : public Resource<sf::Texture>
{
public:
    TextureResource(const std::string& filename);
    ~TextureResource();

    std::string filename() const;
    sf::Texture* data();

    bool isLoaded() const;
private:
    void setData(sf::Texture *data);
};

#endif // TEXTURERESOURCE_HPP
