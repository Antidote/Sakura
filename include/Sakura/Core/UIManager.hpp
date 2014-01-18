#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

namespace Sakura
{
namespace Gui
{
class Container;
} // Gui

namespace Core
{
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
    void addContainer(Gui::Container* c);

    /*!
     * \brief removeContainer
     * \param c
     */
    void removeContainer(Gui::Container* c);

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

    void handleKeyPress(sf::Event::KeyEvent keyEvent);
    void handleKeyRelease(sf::Event::KeyEvent keyEvent);
    void handleMousePress(sf::Event::MouseButtonEvent buttonEvent);
    void handleMouseRelease(sf::Event::MouseButtonEvent buttonEvent);
private:
    std::vector<Gui::Container*> m_containers;
};
} // Core
} // Sakura

#endif // UIMANAGER_HPP
