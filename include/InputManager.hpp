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
    /*!
     * \brief InputManager
     */
    InputManager();
    ~InputManager();

    /*!
     * \brief initialize
     */
    void initialize();
    /*!
     * \brief update
     */
    void update();

    /*!
     * \brief keyboard
     * \return
     */
    KeyboardManager& keyboard();

    /*!
     * \brief mouse
     * \return
     */
    MouseManager& mouse();

    /*!
     * \brief joystick
     * \return
     */
    JoystickManager& joystick();

private:
    KeyboardManager m_keyboardManager;
    MouseManager    m_mouseManager;
    JoystickManager m_joystick;
};

#endif // INPUTMANAGER_HPP
