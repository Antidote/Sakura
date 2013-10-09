#ifndef MUSICRESOURCE_HPP
#define MUSICRESOURCE_HPP

#include "Sakura/Core/Resource.hpp"

#include <SFML/Audio/Music.hpp>
#include "Sakura/Core/PHYSFSStream.hpp"
#include <string>

namespace Sakura
{
namespace Resources
{
class MusicResource : public Core::Resource<sf::Music>
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
    Core::PHYSFSStream m_stream;
};
} // Resources
} // Sakura


#endif // MUSICRESOURCE_HPP
