#ifndef KEYBOARDMANAGER_HPP
#define KEYBOARDMANAGER_HPP

#include "InputBase.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <vector>

class KeyboardManager : public InputBase
{
public:

    /*!
     * \brief KeyboardManager
     */
    KeyboardManager();

    /*!
     * \brief update
     */
    void update();

    /*!
     * \brief pressedKeys
     * \return
     */
    std::vector<sf::Keyboard::Key> pressedKeys();

    /*!
     * \brief isAnyKeyPressed
     * \return
     */
    bool isAnyKeyPressed();

    /*!
     * \brief isAnyKeyReleased
     * \return
     */
    bool isAnyKeyReleased();

    /*!
     * \brief isKeyDown
     * \param key
     * \return
     */
    bool isKeyDown(sf::Keyboard::Key key);

    /*!
     * \brief isKeyUp
     * \param key
     * \return
     */
    bool isKeyUp(sf::Keyboard::Key key);

    /*!
     * \brief wasKeyPressed
     * \param key
     * \return
     */
    bool wasKeyPressed(sf::Keyboard::Key key);

    /*!
     * \brief wasKeyReleased
     * \param key
     * \return
     */
    bool wasKeyReleased(sf::Keyboard::Key key);

    /*!
     * \brief setKey
     * \param key
     * \param pressed
     */
    void setKey(sf::Keyboard::Key key, bool pressed);

    /*!
     * \brief lastPressed
     * \return
     */
    sf::Keyboard::Key lastPressed() const;

    /*!
     * \brief lastReleased
     * \return
     */
    sf::Keyboard::Key lastReleased() const;
private:
    std::map<sf::Keyboard::Key, bool> m_pressedKeys;
    std::map<sf::Keyboard::Key, bool> m_releasedKeys;
    sf::Keyboard::Key       m_lastKeyPressed;
    sf::Keyboard::Key       m_lastKeyReleased;
};
#endif // KEYBOARDMANAGER_HPP
