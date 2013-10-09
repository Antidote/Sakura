#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/Console.hpp"
#include "Sakura/Core/Layer.hpp"
#include "Sakura/Core/Tile.hpp"

namespace Sakura
{
namespace Core
{
Layer::Layer()
{
}

void Layer::setVisible(bool visible)
{
    m_visible = visible;
}

bool Layer::visible() const
{
    return m_visible;
}

void Layer::setZOrder(Uint32 zOrder)
{
    m_zOrder = zOrder;
}

Uint32 Layer::zOrder() const
{
    return m_zOrder;
}

void Layer::addTile(Tile* tile)
{
    for (Tile* t : m_tiles)
    {
        if (tile->position() == t->position())
            return;
    }

    m_tiles.push_back(tile);
}

Tile* Layer::tile(int x, int y)
{
    return tile(sf::Vector2f(x, y));
}

Tile* Layer::tile(const sf::Vector2f& position)
{
    for (Tile* tile : m_tiles)
    {
        if(tile->position() == position)
            return tile;
    }

    return NULL;
}

void Layer::setTiles(const std::vector<Tile*>& tiles)
{
    sEngineRef().console().print(Sakura::Core::Console::Info, "Got %i tiles", tiles.size());
    m_tiles = tiles;
}
} // Core
} // Sakura
