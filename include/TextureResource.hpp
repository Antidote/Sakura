#ifndef TEXTURERESOURCE_HPP
#define TEXTURERESOURCE_HPP

#include "Resource.hpp"
#include <SFML/Graphics/Texture.hpp>

class TextureResource : public Resource<sf::Texture>
{
public:
    TextureResource(const std::string& filename, bool precache = false);
    ~TextureResource();

    std::string filename() const;
    sf::Texture* data();


    void load();
    bool exists() const;
    bool isLoaded() const;
    bool isPrecached() const;
private:
    void setData(sf::Texture *data);
};

#endif // TEXTURERESOURCE_HPP
