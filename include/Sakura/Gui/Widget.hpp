#ifndef WIDGET_HPP
#define WIDGET_HPP


#include <Signal.h>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

namespace Sakura
{
namespace Gui
{
class Container;

class Widget
{
public:
    /*!
     * \brief Signal emitted when a key is pressed.
     */
    typedef Gallant::Signal2<Widget*, sf::Event::KeyEvent>    KeyboardSignal;

    /*!
     * \brief Signal emitted when a mouse button is pressed.
     */
    typedef Gallant::Signal2<Widget*, sf::Event::MouseButtonEvent>    MouseButtonSignal;

    /*!
     * \brief Signal emitted when the mouse is moved.
     */
    typedef Gallant::Signal1<Widget*>  MouseMoveSignal;

    typedef Gallant::Signal1<Widget*> ActivatedSignal;

    /*!
     * \brief Widget
     * \param name
     * \param group
     * \param visible
     * \param enabled
     */
    Widget(Container* owner, const std::string& name, bool visible = true, bool enabled = true);

    /*!
     * \brief Widget
     * \param name
     * \param group
     * \param position
     * \param size
     */
    Widget(Container* owner, const std::string& name, const sf::Vector2f& position, const sf::Vector2f& size);
    virtual ~Widget();

    /*!
     * \brief setOwner
     * \param owner
     */
    void setOwner(Container* owner);

    /*!
     * \brief owner
     * \return
     */
    Container* owner() const;

    /*!
     * \brief name
     * \return
     */
    std::string name() const;

    /*!
     * \brief setPosition
     * \param x
     * \param y
     */
    virtual void setPosition(const float x, const float y);

    /*!
     * \brief setPosition
     * \param position
     */
    virtual void setPosition(const sf::Vector2f& position);

    /*!
     * \brief move
     * \param x
     * \param y
     */
    virtual void move(const float x, const float y);

    /*!
     * \brief move
     * \param pos
     */
    virtual void move(const sf::Vector2f& pos);

    /*!
     * \brief position
     * \return
     */
    sf::Vector2f position() const;

    /*!
     * \brief setSize
     * \param w
     * \param h
     */
    virtual void setSize(const float w, const float h);
    /*!
     * \brief setSize
     * \param size
     */
    virtual void setSize(const sf::Vector2f& size);

    /*!
     * \brief size
     * \return
     */
    sf::Vector2f size() const;


    virtual void setColor(const sf::Color& color);
    sf::Color color() const;
    /*!
     * \brief width
     * \return
     */
    int width() const;

    /*!
     * \brief height
     * \return
     */
    int height() const;

    /*!
     * \brief setVisible
     * \param visible
     */
    void setVisible(bool visible = true);

    /*!
     * \brief visible
     * \return
     */
    bool visible() const;

    /*!
     * \brief show
     */
    void show();

    /*!
     * \brief hide
     */
    void hide();

    /*!
     * \brief setEnabled
     * \param enabled
     */
    void setEnabled(bool enabled = true);

    /*!
     * \brief enabled
     * \return
     */
    bool enabled() const;

    /*!
     * \brief contains
     * \param point
     * \return
     */
    bool contains(const sf::Vector2f& point);

    /*!
     * \brief intersects
     * \param rect
     * \return
     */
    bool intersects(const sf::FloatRect& rect);

    /*!
     * \brief keyPress
     * \return
     */
    KeyboardSignal*    keyPress();

    /*!
     * \brief keyRelese
     * \return
     */
    KeyboardSignal*    keyRelease();

    /*!
     * \brief mousePressed
     * \return
     */
    MouseButtonSignal* mousePressed();

    /*!
     * \brief mouseReleased
     * \return
     */
    MouseButtonSignal* mouseReleased();

    /*!
     * \brief mouseEnter
     * \return
     */
    MouseMoveSignal*   mouseEnter();

    /*!
     * \brief mouseLeave
     * \return
     */
    MouseMoveSignal*   mouseLeave();

    /*!
     * \brief activated
     * \return
     */
    ActivatedSignal*   activated();

    /*!
     * \brief deactivated
     * \return
     */
    ActivatedSignal*   deactivated();
    /*!
     * \brief onKeyPressed
     * \param key
     */
    virtual void onKeyPressed(sf::Event::KeyEvent keyEvent);

    /*!
     * \brief onKeyReleased
     * \param key
     */
    virtual void onKeyReleased(sf::Event::KeyEvent keyEvent);

    /*!
     * \brief onMousePressed
     * \param button
     */
    virtual void onMousePressed(sf::Event::MouseButtonEvent buttonEvent);

    /*!
     * \brief onMouseReleased
     * \param button
     */
    virtual void onMouseReleased(sf::Event::MouseButtonEvent buttonEvent);

    /*!
     * \brief onMouseEnter
     */
    virtual void onMouseEnter();

    /*!
     * \brief onMouseLeave
     */
    virtual void onMouseLeave();

    /*!
     * \brief onActivated
     */
    virtual void onActivate();

    /*!
     * \brief onDeactivate
     */
    virtual void onDeactivate();

    /*!
     * \brief update
     * \param dt
     */
    virtual void update(sf::Time dt);

    /*!
     * \brief draw
     * \param rt
     */
    virtual void draw(sf::RenderTarget& rt);

    /*!
     * \brief activate
     */
    void activate();

    /*!
     * \brief deactivate
     */
    void deactivate();

    virtual void handleKeyPress(sf::Event::KeyEvent keyEvent);
    virtual void handleKeyRelease(sf::Event::KeyEvent keyEvent);
    virtual void handleMousePress(sf::Event::MouseButtonEvent buttonEvent);
    virtual void handleMouseRelease(sf::Event::MouseButtonEvent buttonEvent);

protected:
    Container*        m_owner;
    std::string       m_name;
    bool              m_visible;
    bool              m_enabled;
    sf::Vector2f      m_position;
    sf::Vector2f      m_size;

    KeyboardSignal    m_keyPressSignal;
    KeyboardSignal    m_keyReleaseSignal;
    MouseButtonSignal m_mousePressSignal;
    MouseButtonSignal m_mouseReleaseSignal;
    MouseMoveSignal   m_mouseEnter;
    MouseMoveSignal   m_mouseLeave;
    ActivatedSignal   m_activated;
    ActivatedSignal   m_deactivated;


    sf::Color         m_color;
    // Internal use vars
    bool m_mouseIn;
};
} // Gui
} // Sakura

#endif // WIDGET_HPP
