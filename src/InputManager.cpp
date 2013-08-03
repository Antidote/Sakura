#include "InputManager.hpp"
#include "Engine.hpp"


InputManager::InputManager()
{}

void InputManager::update()
{
    m_keyboardManager.update();
    m_mouseManager.update();
}

MouseManager& InputManager::mouse()
{
    return m_mouseManager;
}


