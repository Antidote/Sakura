#ifndef MOUSEMANAGER_HPP
#define MOUSEMANAGER_HPP

#include "InputBase.hpp"
#include <SFML/Window/Mouse.hpp>
#include <map>
#include <vector>

class MouseManager : public InputBase
{
public:
    /*!
     * \brief MouseManager
     */
    MouseManager();

    /*!
     * \brief update
     */
    void update();

    /*!
     * \brief pressedButtons
     * \return
     */
    std::vector<sf::Mouse::Button> pressedButtons();

    /*!
     * \brief isAnyButtonPressed
     * \return
     */
    bool isAnyButtonPressed();

    /*!
     * \brief isButtonDown
     * \param button
     * \return
     */
    bool isButtonDown(sf::Mouse::Button button);

    /*!
     * \brief isButtonUp
     * \param button
     * \return
     */
    bool isButtonUp(sf::Mouse::Button button);

    /*!
     * \brief wasButtonReleased
     * \param button
     * \return
     */
    bool wasButtonReleased(sf::Mouse::Button button);

    /*!
     * \brief setButtonPressed
     * \param button
     * \param pressed
     */
    void setButtonPressed(sf::Mouse::Button button, bool pressed);

    /*!
     * \brief lastPressed
     * \return
     */
    sf::Mouse::Button lastPressed() const;

    /*!
     * \brief lastReleased
     * \return
     */
    sf::Mouse::Button lastReleased() const;

    /*!
     * \brief mouseLocalPosition
     * \return
     */
    sf::Vector2i mouseLocalPosition();

    /*!
     * \brief mouseGlobalPosition
     * \return
     */
    sf::Vector2i mouseGlobalPosition();

private:
    std::map<sf::Mouse::Button, bool> m_pressedButtons;
    sf::Mouse::Button m_lastButtonPressed;
    sf::Mouse::Button m_lastButtonReleased;
};
#endif // MOUSEMANAGER_HPP
