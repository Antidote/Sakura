#include "KeyboardManager.hpp"
#ifdef SFML_SYSTEM_LINUX
#include <X11/keysym.h>
#endif
#include <mutex>


KeyboardManager::KeyboardManager()
{
    for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
    {
        m_releasedKeys[(sf::Keyboard::Key)i] = m_pressedKeys[(sf::Keyboard::Key)i] = false;
    }
}

void KeyboardManager::update()
{
    static std::mutex mutex;
    mutex.lock();
    for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
    {
        sf::Keyboard::Key key = (sf::Keyboard::Key)i;
        m_pressedKeys[key] = sf::Keyboard::isKeyPressed(key);

        m_releasedKeys[key] = !m_pressedKeys[key];

        if (m_lastKeyPressed == key && !m_pressedKeys[m_lastKeyPressed])
            m_lastKeyReleased = m_lastKeyPressed;

        if (m_pressedKeys[key])
            m_lastKeyPressed = key;
    }

    // Check the tilde key
    // SFML Currently doesn't properly support the '`' key
    // (XK_grave and XK_dead_grave) on some systems, this is a work around.
    // Why this key in particular is such a pain in the neck is beyond me.
#if defined(SFML_SYSTEM_LINUX) && defined(pie)
    // TODO: Handle it
    if (unicode == XK_grave || unicode == XK_dead_grave)
    {
        ;
    }
#endif

    mutex.unlock();
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
    return (!sf::Keyboard::isKeyPressed(key) && m_pressedKeys[key]);
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
