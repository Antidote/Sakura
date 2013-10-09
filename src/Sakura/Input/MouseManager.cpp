#include "Sakura/Core/Engine.hpp"
#include "Sakura/Input/MouseManager.hpp"

namespace Sakura
{
namespace Input
{
MouseManager::MouseManager()
{
    for (int i = 0; i < sf::Mouse::ButtonCount - 1; i++)
    {
        m_pressedButtons[(sf::Mouse::Button)i] = false;
    }
}

void MouseManager::update()
{
    static std::mutex mutex;
    mutex.lock();
    m_lastButtonReleased = sf::Mouse::ButtonCount;
    for (int i = 0; i < sf::Mouse::ButtonCount - 1; i++)
    {
        sf::Mouse::Button btn = (sf::Mouse::Button)i;
        m_pressedButtons[btn] = sf::Mouse::isButtonPressed(btn);

        if (m_lastButtonPressed == btn && !m_pressedButtons[btn])
            m_lastButtonReleased = m_lastButtonPressed;

        if (m_pressedButtons[btn])
            m_lastButtonPressed = (sf::Mouse::Button)i;
    }

    mutex.unlock();
}

std::vector<sf::Mouse::Button> MouseManager::pressedButtons()
{
    std::vector<sf::Mouse::Button> ret;
    for (int i = 0; i < sf::Mouse::ButtonCount - 1; i++)
    {
        if (m_pressedButtons[(sf::Mouse::Button)i])
            ret.push_back((sf::Mouse::Button)i);
    }

    return ret;
}

std::vector<sf::Mouse::Button> MouseManager::releasedButtons()
{
    std::vector<sf::Mouse::Button> ret;
    for (int i = 0; i < sf::Mouse::ButtonCount - 1; i++)
    {
        if (!sf::Mouse::isButtonPressed((sf::Mouse::Button)i) && m_pressedButtons[(sf::Mouse::Button)i])
            ret.push_back((sf::Mouse::Button)i);
    }

    return ret;
}

bool MouseManager::isAnyButtonPressed()
{
    return pressedButtons().size() > 0;
}

bool MouseManager::isAnyButtonReleased()
{
    return releasedButtons().size() > 0;
}

bool MouseManager::isButtonDown(sf::Mouse::Button button)
{
    return m_pressedButtons[button];
}

bool MouseManager::isButtonUp(sf::Mouse::Button button)
{
    return !m_pressedButtons[button];
}

bool MouseManager::wasButtonPressed(sf::Mouse::Button button)
{
    return (sf::Mouse::isButtonPressed(button) && !m_pressedButtons[button]);
}

bool MouseManager::wasButtonReleased(sf::Mouse::Button button)
{
    return (!sf::Mouse::isButtonPressed(button) && m_pressedButtons[button]);
}

void MouseManager::setButtonPressed(sf::Mouse::Button button, bool pressed)
{
    m_pressedButtons[button] = pressed;
}

sf::Mouse::Button MouseManager::lastPressed() const
{
    return m_lastButtonPressed;
}

sf::Mouse::Button MouseManager::lastReleased() const
{
    return m_lastButtonReleased;
}

sf::Vector2i MouseManager::mouseLocalPosition()
{
    return sf::Mouse::getPosition(sEngineRef().window());
}

sf::Vector2i MouseManager::mouseGlobalPosition()
{
    return sf::Mouse::getPosition();
}
} // Input
} // Sakura
