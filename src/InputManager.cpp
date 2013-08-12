#include "InputManager.hpp"
#include "Engine.hpp"



InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::initialize()
{

}

void InputManager::update()
{
    m_keyboardManager.update();
    m_mouseManager.update();
    m_joystick.update();
}

KeyboardManager& InputManager::keyboard()
{
    return m_keyboardManager;
}


MouseManager& InputManager::mouse()
{
    return m_mouseManager;
}

JoystickManager& InputManager::joystick()
{
    return m_joystick;
}


