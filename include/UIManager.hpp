#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

class Container;

class UIManager
{
public:

    /*!
     * \brief UIManager
     */
    UIManager();
    ~UIManager();

    /*!
     * \brief addContainer
     * \param c
     */
    void addContainer(Container* c);

    /*!
     * \brief removeContainer
     * \param c
     */
    void removeContainer(Container* c);

    /*!
     * \brief update
     * \param dt
     */
    void update(sf::Time dt);

    /*!
     * \brief draw
     * \param rt
     */
    void draw(sf::RenderTarget& rt);

    void handleInput(sf::Keyboard::Key code, bool alt, bool control, bool shift, bool system);
private:
    std::vector<Container*> m_containers;
};

#endif // UIMANAGER_HPP
