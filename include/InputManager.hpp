#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "KeyboardManager.hpp"
#include "MouseManager.hpp"
#include "JoystickManager.hpp"
#include <thread>
#include <mutex>

class InputManager
{
public:
    InputManager();
    ~InputManager();

    void initialize();
    void update();

    KeyboardManager& keyboard();
    MouseManager& mouse();
    JoystickManager& joystick();

private:
    KeyboardManager m_keyboardManager;
    MouseManager    m_mouseManager;
    JoystickManager m_joystick;
};

#endif // INPUTMANAGER_HPP
