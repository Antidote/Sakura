#include "Sakura/Core/Player.hpp"
#include "Sakura/Core/Engine.hpp"

namespace Sakura
{
namespace Core
{
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
} // Core
} // Sakura
