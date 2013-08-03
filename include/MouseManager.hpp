#ifndef MOUSEMANAGER_HPP
#define MOUSEMANAGER_HPP

#include "InputBase.hpp"
#include <SFML/Window/Mouse.hpp>
#include <map>
#include <vector>

class MouseManager : public InputBase
{
public:
    MouseManager();
    void update();

    std::vector<sf::Mouse::Button> pressedButtons();
    bool isAnyButtonPressed();
    bool isButtonDown(sf::Mouse::Button button);
    bool isButtonUp(sf::Mouse::Button button);
    bool wasButtonReleased(sf::Mouse::Button button);

    void setButtonPressed(sf::Mouse::Button button, bool pressed);

    sf::Mouse::Button lastPressed() const;
    sf::Mouse::Button lastReleased() const;

    sf::Vector2i mouseLocalPosition();
    sf::Vector2i mouseGlobalPosition();

private:
    std::map<sf::Mouse::Button, bool> m_pressedButtons;
    sf::Mouse::Button m_lastButtonPressed;
    sf::Mouse::Button m_lastButtonReleased;
};
#endif // MOUSEMANAGER_HPP
