#ifndef SOUNDRESOURCE_HPP
#define SOUNDRESOURCE_HPP

#include "Resource.hpp"
#include <SFML/Audio/SoundBuffer.hpp>

class SoundResource : public Resource<sf::SoundBuffer>
{
public:
    SoundResource(const std::string& filename);
    ~SoundResource();

    std::string filename() const;
    sf::SoundBuffer* data();

    bool isLoaded() const;
private:
    void setData(sf::SoundBuffer *data);
};

#endif // SOUNDRESOURCE_HPP
