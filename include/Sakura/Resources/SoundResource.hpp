#ifndef SOUNDRESOURCE_HPP
#define SOUNDRESOURCE_HPP

#include "Sakura/Core/Resource.hpp"
#include <SFML/Audio/SoundBuffer.hpp>

namespace Sakura
{
namespace Resources
{

class SoundResource : public Core::Resource<sf::SoundBuffer>
{
public:
    /*!
     * \brief SoundResource
     * \param filename
     * \param precache
     */
    SoundResource(const std::string& filename, bool precache = true);
    ~SoundResource();

    /*!
     * \brief data
     * \return
     */
    sf::SoundBuffer* data() const;

    /*!
     * \brief filename
     * \return
     */
    std::string filename() const;

    /*!
     * \brief isLoaded
     * \return
     */
    bool isLoaded() const;

    /*!
     * \brief exists
     * \return
     */
    bool exists() const;

    /*!
     * \brief isPrecached
     * \return
     */
    bool isPrecached();

    /*!
     * \brief load
     */
    void load();
private:
    void setData(sf::SoundBuffer *data);
};

} // Resources
} // Sakura

#endif // SOUNDRESOURCE_HPP
