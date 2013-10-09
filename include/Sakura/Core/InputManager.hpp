#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "Sakura/Input/KeyboardManager.hpp"
#include "Sakura/Input/MouseManager.hpp"
#include "Sakura/Input/JoystickManager.hpp"
#include <thread>
#include <mutex>

namespace Sakura
{
namespace Core
{
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
    Input::KeyboardManager& keyboard();

    /*!
     * \brief mouse
     * \return
     */
    Input::MouseManager& mouse();

    /*!
     * \brief joystick
     * \return
     */
    Input::JoystickManager& joystick();

private:
    Input::KeyboardManager m_keyboardManager;
    Input::MouseManager    m_mouseManager;
    Input::JoystickManager m_joystick;
};
} // Core
} // Sakura


#endif // INPUTMANAGER_HPP
