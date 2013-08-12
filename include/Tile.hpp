#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

class Tile
{
public:
    enum CollisionType
    {
        None = -1, // No collision (i.e air)
        Solid,
        Angle45
    };

    Tile(const sf::Vector2f& position, const sf::Vector2f size, const CollisionType colType);
    sf::FloatRect bounds() const;

    sf::Vector2f position() const;
    sf::Vector2f size() const;

    CollisionType collisionType () const;
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    CollisionType m_collisionType;
};

#endif // TILE_HPP
