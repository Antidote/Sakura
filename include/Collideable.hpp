#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

#include <SFML/Graphics.hpp>

class Entity;

class Collideable
{
public:
    /*!
     * \brief collide
     * \param entity
     */
    virtual void collide(Entity* entity);

    /*!
     * \brief collisionRect
     * \return
     */
    sf::IntRect collisionRect() const;
private:
    sf::IntRect m_collisionRect;
};

#endif // COLLIDABLE_HPP
