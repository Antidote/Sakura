#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/GameState.hpp"

namespace Sakura
{
namespace Core
{
GameState::GameState()
    : m_initialized(false)
{}

void GameState::initialize()
{
    // do loading of resources here
    m_initialized = true;
}

void GameState::update(sf::Time dt)
{
    UNUSED(dt);
}

void GameState::draw(sf::RenderTarget& rt)
{
    UNUSED(rt);
}

bool GameState::isInitialized() const
{
    return m_initialized;
}

RunState::Type GameState::type() const
{
    return Game;
}

bool GameState::isDone() const
{
    return false;
}

std::string GameState::name() const
{
    return "game";
}

RunState* GameState::nextState() const
{
    return NULL;
}
} // Core
} // Sakura
