#ifndef MAP_HPP
#define MAP_HPP

class Layer;

#include <unordered_map>
#include <vector>
#include <string>

#include "Types.hpp"

struct RGBA
{
    Uint8 B;
    Uint8 G;
    Uint8 R;
    Uint8 A;
};

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

struct Cell
{
    Uint8 CollisionType;
    Uint8 FlippedAndDamage;
};

class Map
{
public:
    static const int MAGIC_NUMBER     = ('Z' | ('M' << 8) | ('A' << 16) | ('P' << 24));
    static const int VERSION_MAJOR    = 1;
    static const int VERSION_MINOR    = 2;
    static const int VERSION_REVISION = 0;
    static const int VERSION_PATCH    = 1;
    static const int VERSION          = (VERSION_MAJOR | (VERSION_MINOR << 8) | (VERSION_REVISION << 16) | (VERSION_PATCH << 24));

    Map();


    void setName(const std::string& name);
    std::string name() const;


    void addTileset(const std::string& tileset);
    std::string tileset(Uint32 id) const;
    bool tilesetExists(const std::string& tileset) const;

    void addLayer(Layer* layer);
    Layer* layer(int id) const;
    bool layerExists(Layer* layer) const;

    void setWidth(Uint32 width);
    Uint32 width() const;

    void setHeight(Uint32 height);
    Uint32 height() const;

    void setTileWidth(Uint32 width);
    Uint32 tileWidth() const;

    void setTileHeight(Uint32 height);
    Uint32 tileHeight() const;

    void setBackgroundColor(RGBA color);
    RGBA backgroundColor() const;

    void setCollisionData(std::unordered_map<int, std::unordered_map<int, Cell*> >  data);
    Cell* collision(int x, int y);
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
