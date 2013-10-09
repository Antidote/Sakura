#include "Sakura/Core/Engine.hpp"
#include "Sakura/Input/JoystickManager.hpp"
#include <iostream>

namespace Sakura
{
namespace Input
{
JoystickManager::JoystickManager()
{
    sf::Joystick::update();
    for (int i = 0; i < sf::Joystick::Count; i++)
    {
        m_joysticks.push_back(new Joystick(i));
    }
}

void JoystickManager::update()
{
    for (Joystick* jstick : m_joysticks)
        jstick->update();

}

bool JoystickManager::isButtonPressed(int joystick, int button) const
{
    if (joystick > (int)m_joysticks.size())
        return false;

    return m_joysticks[joystick]->isButtonPressed(button);
}

bool JoystickManager::wasButtonReleased(int joystick, int button) const
{
    if (joystick > (int)m_joysticks.size())
        return false;

    return m_joysticks[joystick]->wasButtonReleased(button);
}


JoystickManager::Joystick::Joystick(int id)
    : m_id(id)
{
    for (int i = 0; i < sf::Joystick::AxisCount; i++)
    {
        m_hasAxis.push_back(false);
        m_axisPos.push_back(0.0f);
    }

    for (int i = 0; i < sf::Joystick::ButtonCount; i++)
    {
        m_buttons.push_back(false);
        m_releasedButtons.push_back(false);
    }
}

JoystickManager::Joystick::~Joystick()
{
    m_hasAxis.clear();
    m_axisPos.clear();
    m_buttons.clear();
}

void JoystickManager::Joystick::update()
{
    if (!sf::Joystick::isConnected(m_id))
        return;
    static std::mutex mutex;
    mutex.lock();

    for (int i = 0; i < sf::Joystick::AxisCount; i++)
    {
        m_hasAxis[i] = sf::Joystick::hasAxis(m_id, (sf::Joystick::Axis)i);
        if (m_hasAxis[i])
            m_axisPos[i] = sf::Joystick::getAxisPosition(m_id, (sf::Joystick::Axis)i);
    }

    for (int i = 0; i < (int)sf::Joystick::getButtonCount(m_id); i++)
    {
        m_releasedButtons[i] = (m_buttons[i] && !sf::Joystick::isButtonPressed(m_id, i));
        m_buttons[i] = sf::Joystick::isButtonPressed(m_id, i);
    }

    mutex.unlock();
}

bool JoystickManager::Joystick::hasAxis(sf::Joystick::Axis axis) const
{
    if (!sf::Joystick::isConnected(m_id))
        return false;

    if (axis > (int)m_hasAxis.size())
        return false;

    return m_hasAxis[axis];
}

float JoystickManager::Joystick::axisPosition(sf::Joystick::Axis axis) const
{
    if (!sf::Joystick::isConnected(m_id))
        return 0.0f;

    if (axis > (int)m_axisPos.size() || !m_hasAxis[axis])
        return 0.0f;

    return m_axisPos[axis];
}

bool JoystickManager::Joystick::isButtonPressed(int button) const
{
    if (!sf::Joystick::isConnected(m_id))
        return false;
    if (button > (int)m_buttons.size())
        return false;

    return m_buttons[button];
}

bool JoystickManager::Joystick::wasButtonReleased(int button) const
{
    if (!sf::Joystick::isConnected(m_id))
        return false;

    if (button > (int)m_releasedButtons.size())
        return false;

    return m_releasedButtons[button];
}
} // Input
} // Sakura
