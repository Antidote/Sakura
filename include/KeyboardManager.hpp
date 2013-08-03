#ifndef KEYBOARDMANAGER_HPP
#define KEYBOARDMANAGER_HPP

#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <vector>

class KeyboardManager
{
public:
    KeyboardManager();
    void update();

    std::vector<sf::Keyboard::Key> pressedKeys();
    bool isAnyKeyPressed();
    bool isKeyDown(sf::Keyboard::Key key);
    bool isKeyUp(sf::Keyboard::Key key);
    bool wasKeyReleased(sf::Keyboard::Key key);

    void setKey(sf::Keyboard::Key key, bool pressed);

    sf::Keyboard::Key lastPressed() const;
    sf::Keyboard::Key lastReleased() const;
private:
    std::map<sf::Keyboard::Key, bool> m_pressedKeys;
    sf::Keyboard::Key       m_lastKeyPressed;
    sf::Keyboard::Key       m_lastKeyReleased;
};
#endif // KEYBOARDMANAGER_HPP
