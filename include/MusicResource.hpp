#ifndef MUSICRESOURCE_HPP
#define MUSICRESOURCE_HPP

#include "Resource.hpp"

#include <SFML/Audio/Music.hpp>
#include <string>

class MusicResource : public Resource<sf::Music>
{
public:
    /*!
     * \brief MusicResource
     * \param precache
     */
    MusicResource(const std::string&, bool precache = false);
    ~MusicResource();

    /*!
     * \brief filename
     * \return
     */
    std::string filename() const;

    /*!
     * \brief data
     * \return
     */
    sf::Music* data();

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
    void setData(sf::Music *data);
};

#endif // MUSICRESOURCE_HPP
