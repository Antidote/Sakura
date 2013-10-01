#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Audio.hpp>
#include "Entity.hpp"
#include "Killable.hpp"
#include "AnimatedSprite.hpp"
#include "Collideable.hpp"

class Cell;

class Player : public Entity, public Killable, public Collideable
{
public:
    enum State
    {
        Idle,
        Moving,
        Lifting,
        Pushing,
        Pulling
    };

    Player();
};

#endif // LINK_HPP
