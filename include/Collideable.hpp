#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

#include <SFML/Graphics.hpp>

class Entity;

class Collideable
{
public:
    virtual void collide(Entity* entity);

    sf::IntRect collisionRect() const;
private:
    sf::IntRect m_collisionRect;
};

#endif // COLLIDABLE_HPP
