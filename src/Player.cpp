#include "Player.hpp"
#include "Engine.hpp"
#include "SoundResource.hpp"
#include "TextureResource.hpp"
#include "Collideable.hpp"
#include "Map.hpp"
#include <iostream>

int Player::PlayerCount = 0;

Player::Player()
    : Entity("Player", Entity::Player)
{
    m_playerId = ++PlayerCount;
}

int Player::playerId() const
{
    return m_playerId;
}
