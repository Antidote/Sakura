#include "Player.hpp"
#include "Engine.hpp"
#include "SoundResource.hpp"
#include "TextureResource.hpp"
#include "Collideable.hpp"
#include "Map.hpp"
#include <iostream>

Player::Player()
    : Entity("Player", Entity::Player)
{
}
