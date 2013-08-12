#ifndef SOUNDRESOURCE_HPP
#define SOUNDRESOURCE_HPP

#include "Resource.hpp"
#include <SFML/Audio/SoundBuffer.hpp>

class SoundResource : public Resource<sf::SoundBuffer>
{
public:
    SoundResource(const std::string& filename, bool precache = true);
    ~SoundResource();

    sf::SoundBuffer* data() const;
    std::string filename() const;

    bool isLoaded() const;
    bool exists() const;
    bool isPrecached();
    void load();
private:
    void setData(sf::SoundBuffer *data);
};

#endif // SOUNDRESOURCE_HPP
