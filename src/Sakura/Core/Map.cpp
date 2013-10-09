#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/Map.hpp"
#include "Sakura/Core/ResourceManager.hpp"
#include "Sakura/Resources/TextureResource.hpp"
#include <stdio.h>

namespace Sakura
{
namespace Core
{
Map::Map()
    : m_width(0),
      m_height(0),
      m_tileWidth(0),
      m_tileHeight(0)
{
}

void Map::setName(const std::string& name)
{
    m_name = name;
}

std::string Map::name() const
{
    return m_name;
}

void Map::addTileset(const std::string& tileset)
{
    if (!tilesetExists(tileset))
        m_tilesets.push_back(tileset);
}

bool Map::tilesetExists(const std::string& tileset) const
{
    std::vector<std::string>::const_iterator iter = m_tilesets.cbegin();

    for (; iter != m_tilesets.end(); ++iter)
    {
        if (!((std::string)*iter).compare(tileset))
            return true;
    }

    return false;
}

void Map::addLayer(Layer* layer)
{
    if (!layerExists(layer))
        m_layers.push_back(layer);
}

Layer* Map::layer(int id) const
{
    if (id <= 0 || id > (int)m_layers.size())
        return NULL;

    return m_layers[id];
}

bool Map::layerExists(Layer* layer) const
{
    std::vector<Layer*>::const_iterator iter = m_layers.cbegin();

    for (; iter != m_layers.end(); ++iter)
    {
        if (*iter == layer)
            return true;
    }

    return false;
}

void Map::setWidth(Uint32 width)
{
    m_width = width;
}

Uint32 Map::width() const
{
    return m_width;
}

void Map::setHeight(Uint32 height)
{
    m_height = height;
}

Uint32 Map::height() const
{
    return m_height;
}

void Map::setTileWidth(Uint32 width)
{
    m_tileWidth = width;
}

Uint32 Map::tileWidth() const
{
    return m_tileWidth;
}

void Map::setTileHeight(Uint32 height)
{
    m_tileHeight = height;
}

Uint32 Map::tileHeight() const
{
    return m_tileHeight;
}

void Map::setBackgroundColor(RGBA color)
{
    m_bgColor = color;
}

RGBA Map::backgroundColor() const
{
    return m_bgColor;
}

void Map::setCollisionData(std::unordered_map<int, std::unordered_map<int, Cell*> > data)
{
    m_collisionData = data;
}

Cell* Map::collision(int x, int y)
{
    if (m_collisionData.find(x) != m_collisionData.end() && m_collisionData[x].find(y) != m_collisionData[x].end())
        return m_collisionData[x][y];

    return NULL;
}

void Map::draw(sf::RenderTarget& rt)
{
    UNUSED(rt);
    /*
    static ResourceManager& resourceManager = Engine::instance().resourceManager();
    for (int i = 0; i < m_tilesets.size(); ++i)
    {
        if (!resourceManager.textureExists(m_tilesets[i]))
            resourceManager.loadTexture(m_tilesets[i], new TextureResource(m_tilesets[i], true));

        for (Layer* layer : m_layers)
            layer->draw(resourceManager.texture(m_tilesets[i]), rt);
    }*/
}
} // Core
} // Sakura

