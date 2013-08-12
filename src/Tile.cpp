#include "Tile.hpp"

Tile::Tile(const sf::Vector2f& position, const sf::Vector2f size, const Tile::CollisionType colType)
    : m_position(position),
      m_size(size),
      m_collisionType(colType)
{
}

sf::FloatRect Tile::bounds() const
{
    return sf::FloatRect(m_position, m_size);
}

sf::Vector2f Tile::position() const
{
    return m_position;
}

sf::Vector2f Tile::size() const
{
    return m_size;
}

Tile::CollisionType Tile::collisionType() const
{
    return m_collisionType;
}
