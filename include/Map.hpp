#ifndef MAP_HPP
#define MAP_HPP

class Layer;

#include <unordered_map>
#include <vector>
#include <string>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Types.hpp"

/*!
 * \brief The RGBA struct
 */
struct RGBA
{
    Uint8 B;
    Uint8 G;
    Uint8 R;
    Uint8 A;
};

/*!
 * \brief The ColType enum
 */
enum ColType
{
    ColTypeNone         = 0x00,
    ColTypeSolid        = 0x01,
    ColTypeAngle45      = 0x02,
    ColTypeAngle25      = 0x04,
    ColTypeJump         = 0x08,
    ColTypeWaterShallow = 0x10,
    ColTypeWaterDeep    = 0x20,
    ColTypeDamage       = 0x40,
    ColTypeStair        = 0x80
};

/*!
 * \brief The Cell struct
 */
struct Cell
{
    Uint8 CollisionType;
    Uint8 FlippedAndDamage;
};

/*!
 * \brief The Map class
 */
class Map
{
public:
    /*!
     * \brief MAGIC_NUMBER
     */
    static const int MAGIC_NUMBER     = ('Z' | ('M' << 8) | ('A' << 16) | ('P' << 24));
    /*!
     * \brief VERSION_MAJOR
     */
    static const int VERSION_MAJOR    = 1;
    /*!
     * \brief VERSION_MINOR
     */
    static const int VERSION_MINOR    = 2;
    /*!
     * \brief VERSION_REVISION
     */
    static const int VERSION_REVISION = 0;
    /*!
     * \brief VERSION_PATCH
     */
    static const int VERSION_PATCH    = 1;
    /*!
     * \brief VERSION
     */
    static const int VERSION          = (VERSION_MAJOR | (VERSION_MINOR << 8) | (VERSION_REVISION << 16) | (VERSION_PATCH << 24));

    /*!
     * \brief Map
     */
    Map();

    /*!
     * \brief setName
     * \param name
     */
    void setName(const std::string& name);

    /*!
     * \brief name
     * \return
     */
    std::string name() const;

    /*!
     * \brief addTileset
     * \param tileset
     */
    void addTileset(const std::string& tileset);

    /*!
     * \brief tileset
     * \param id
     * \return
     */
    std::string tileset(Uint32 id) const;

    /*!
     * \brief tilesetExists
     * \param tileset
     * \return
     */
    bool tilesetExists(const std::string& tileset) const;

    /*!
     * \brief addLayer
     * \param layer
     */
    void addLayer(Layer* layer);

    /*!
     * \brief layer
     * \param id
     * \return
     */
    Layer* layer(int id) const;

    /*!
     * \brief layerExists
     * \param layer
     * \return
     */
    bool layerExists(Layer* layer) const;

    /*!
     * \brief setWidth
     * \param width
     */
    void setWidth(Uint32 width);

    /*!
     * \brief width
     * \return
     */
    Uint32 width() const;

    /*!
     * \brief setHeight
     * \param height
     */
    void setHeight(Uint32 height);

    /*!
     * \brief height
     * \return
     */
    Uint32 height() const;

    /*!
     * \brief setTileWidth
     * \param width
     */
    void setTileWidth(Uint32 width);

    /*!
     * \brief tileWidth
     * \return
     */
    Uint32 tileWidth() const;

    /*!
     * \brief setTileHeight
     * \param height
     */
    void setTileHeight(Uint32 height);

    /*!
     * \brief tileHeight
     * \return
     */
    Uint32 tileHeight() const;

    /*!
     * \brief setBackgroundColor
     * \param color
     */
    void setBackgroundColor(RGBA color);

    /*!
     * \brief backgroundColor
     * \return
     */
    RGBA backgroundColor() const;

    /*!
     * \brief setCollisionData
     * \param data
     */
    void setCollisionData(std::unordered_map<int, std::unordered_map<int, Cell*> >  data);

    /*!
     * \brief collision
     * \param x
     * \param y
     * \return
     */
    Cell* collision(int x, int y);

    /*!
     * \brief draw
     * \param rt
     */
    void draw(sf::RenderTarget& rt);
private:
    std::string              m_name;
    std::vector<Layer*>      m_layers;
    std::vector<std::string> m_tilesets;
    // X -> Y -> Cell
    std::unordered_map<int, std::unordered_map<int, Cell*> >    m_collisionData;
    RGBA                     m_bgColor;

    Uint32                   m_width;
    Uint32                   m_height;
    Uint32                   m_tileWidth;
    Uint32                   m_tileHeight;
};

#endif // MAP_HPP
