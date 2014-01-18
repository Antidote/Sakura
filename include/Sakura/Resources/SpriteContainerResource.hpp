#ifndef SPRITECONTAINERRESOURCE_HPP
#define SPRITECONTAINERRESOURCE_HPP

#include "Sakura/Core/ResourceBase.hpp"
#include "Sakura/Core/SSpriteFile.hpp"

namespace Sakura
{
namespace Resources
{

class SpriteContainerResource : public Core::ResourceBase<Core::SSpriteFile>
{
public:
    SpriteContainerResource(const std::string& filename, bool precache);
    ~SpriteContainerResource();

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

#endif // SPRITECONTAINERRESOURCE_HPP
