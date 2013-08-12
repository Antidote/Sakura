#ifndef MUSICRESOURCE_HPP
#define MUSICRESOURCE_HPP

#include "Resource.hpp"

#include <SFML/Audio/Music.hpp>
#include <string>

class MusicResource : public Resource<sf::Music>
{
public:
    MusicResource(const std::string&, bool precache = false);
    ~MusicResource();


    std::string filename() const;
    sf::Music* data();

    void load();

    bool exists() const;
    bool isLoaded() const;
    bool isPrecached() const;
private:
    void setData(sf::Music *data);
};

#endif // MUSICRESOURCE_HPP
