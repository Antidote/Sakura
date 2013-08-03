#include "Collideable.hpp"


void Collideable::collide(Entity* entity)
{
    ((void)entity);
}

sf::IntRect Collideable::collisionRect() const
{
    return m_collisionRect;
}
