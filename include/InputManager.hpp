#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "KeyboardManager.hpp"
#include "MouseManager.hpp"

class InputManager
{
public:
    InputManager();
    void update();

    KeyboardManager& keyboard();
    MouseManager& mouse();

private:
    KeyboardManager m_keyboardManager;
    MouseManager m_mouseManager;
};

#endif // INPUTMANAGER_HPP
