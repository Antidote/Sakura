#include "Sakura/Core/Tile.hpp"

namespace Sakura
{
namespace Core
{
Tile::Tile()
{

}

void Tile::setId(Uint32 id)
{
    m_id = id;
}

Uint32 Tile::id() const
{
    return m_id;
}

void Tile::setFlippedHor(bool flipped)
{
    m_flippedHor = flipped;
}

bool Tile::flippedHor() const
{
    return m_flippedHor;
}

void Tile::setFlippedVer(bool flipped)
{
    m_flippedVer = flipped;
}

bool Tile::flippedVer() const
{
    return m_flippedVer;
}

void Tile::setFlippedDiag(bool flipped)
{
    m_flippedDiag = flipped;
}

bool Tile::flippedDiag() const
{
    return m_flippedDiag;
}

void Tile::setPosition(float x, float y)
{
    setPosition(sf::Vector2f(x, y));
}

void Tile::setPosition(const sf::Vector2f& position)
{
    m_position = position;
}

sf::Vector2f Tile::position() const
{
    return m_position;
}

void Tile::setTileset(int id)
{
    m_tileset = id;
}

Uint32 Tile::tileset() const
{
    return m_tileset;
}
} // Core
} // Sakura

