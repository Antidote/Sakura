#include "KeyboardManager.hpp"



KeyboardManager::KeyboardManager()
{
    for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
    {
        m_pressedKeys[(sf::Keyboard::Key)i] = false;
    }
}

void KeyboardManager::update()
{
    for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
    {
        sf::Keyboard::Key key = (sf::Keyboard::Key)i;
        m_pressedKeys[key] = sf::Keyboard::isKeyPressed(key);

        if (m_lastKeyPressed == key && !m_pressedKeys[m_lastKeyPressed])
            m_lastKeyReleased = m_lastKeyPressed;

        if (m_pressedKeys[key])
            m_lastKeyPressed = key;
    }
}

std::vector<sf::Keyboard::Key> KeyboardManager::pressedKeys()
{
    std::vector<sf::Keyboard::Key> ret;
    for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
    {
        if (m_pressedKeys[(sf::Keyboard::Key)i])
            ret.push_back((sf::Keyboard::Key)i);
    }

    return ret;
}

bool KeyboardManager::isAnyKeyPressed()
{
    return pressedKeys().size() > 0;
}

bool KeyboardManager::isKeyDown(sf::Keyboard::Key key)
{
    return m_pressedKeys[key];
}

bool KeyboardManager::isKeyUp(sf::Keyboard::Key key)
{
    return !m_pressedKeys[key];
}

bool KeyboardManager::wasKeyReleased(sf::Keyboard::Key key)
{
    return (!sf::Keyboard::isKeyPressed(key) && !m_pressedKeys[key]);
}

void KeyboardManager::setKey(sf::Keyboard::Key key, bool pressed)
{
    m_pressedKeys[key] = pressed;
}

sf::Keyboard::Key KeyboardManager::lastPressed() const
{
    return m_lastKeyPressed;
}

sf::Keyboard::Key KeyboardManager::lastReleased() const
{
    return m_lastKeyReleased;
}
