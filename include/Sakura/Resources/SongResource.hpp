#ifndef SONGRESOURCE_HPP
#define SONGRESOURCE_HPP

#include "Sakura/Core/ResourceBase.hpp"

#include <SFML/Audio/Music.hpp>
#include "Sakura/Core/PHYSFSStream.hpp"
#include <string>

namespace Sakura
{
namespace Resources
{
class SongResource : public Core::ResourceBase<sf::Music>
{
public:
    /*!
     * \brief SongResource
     * \param precache
     */
    SongResource(const std::string&, bool precache = false);
    ~SongResource();

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


#endif // SONGRESOURCE_HPP
