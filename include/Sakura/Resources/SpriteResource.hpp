#ifndef SPRITERESOURCE_HPP
#define SPRITERESOURCE_HPP

#include "Sakura/Core/ResourceBase.hpp"
#include "Sakura/Core/SSpriteFile.hpp"

namespace Sakura
{
namespace Resources
{

class SpriteResource : public Core::ResourceBase<Core::SSpriteFile>
{
public:
    SpriteResource(const std::string& filename, bool precache);
    ~SpriteResource();

    /*!
     * \brief filename
     * \return
     */
    std::string filename() const;

    /*!
     * \brief data
     * \return
     */
    Core::SSpriteFile* data();

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
    bool isPrecached();
private:
    void setData(Core::SSpriteFile *data);
};

}
}

#endif // SPRITERESOURCE_HPP
