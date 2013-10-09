#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/InputManager.hpp"

namespace Sakura
{
namespace Core
{
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

Input::KeyboardManager& InputManager::keyboard()
{
    return m_keyboardManager;
}

Input::MouseManager& InputManager::mouse()
{
    return m_mouseManager;
}

Input::JoystickManager& InputManager::joystick()
{
    return m_joystick;
}

} // Core
} // Sakura

