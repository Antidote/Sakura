#ifndef HEARTPICKUP_HPP
#define HEARTPICKUP_HPP

#include "Entity.hpp"
#include "Collideable.hpp"
#include "AnimatedSprite.hpp"
#include <SFML/Audio.hpp>

class HeartPickup : public Entity, public Collideable
{
public:
    HeartPickup();

    void update(sf::Time delta);
    void draw(sf::RenderTarget &rt);

    void collide(Entity *entity);
private:
    Animation      m_animation;
    AnimatedSprite m_sprite;
};

#endif // HEARTPICKUP_HPP
