#ifndef WIDGET_HPP
#define WIDGET_HPP


#include <Signal.h>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
class Container;



class Widget
{
public:
    /*!
     * \brief Signal emitted when a key is pressed.
     */
    typedef Gallant::Signal2<Widget*, sf::Keyboard::Key>    KeyboardSignal;

    /*!
     * \brief Signal emitted when a mouse button is pressed.
     */
    typedef Gallant::Signal2<Widget*, sf::Mouse::Button>    MouseButtonSignal;

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
    Widget(const std::string& name, bool visible = true, bool enabled = true);

    /*!
     * \brief Widget
     * \param name
     * \param group
     * \param position
     * \param size
     */
    Widget(const std::string& name, const sf::Vector2f& position, const sf::Vector2f& size);
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
    void setPosition(const float x, const float y);

    /*!
     * \brief setPosition
     * \param position
     */
    void setPosition(const sf::Vector2f& position);

    /*!
     * \brief move
     * \param x
     * \param y
     */
    void move(const float x, const float y);

    /*!
     * \brief move
     * \param pos
     */
    void move(const sf::Vector2f& pos);

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
    void setSize(const float w, const float h);
    /*!
     * \brief setSize
     * \param size
     */
    void setSize(const sf::Vector2f& size);

    /*!
     * \brief size
     * \return
     */
    sf::Vector2f size() const;


    void setColor(const sf::Color& color);
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
    virtual void onKeyPressed(sf::Keyboard::Key key);

    /*!
     * \brief onKeyReleased
     * \param key
     */
    virtual void onKeyReleased(sf::Keyboard::Key key);

    /*!
     * \brief onMousePressed
     * \param button
     */
    virtual void onMousePressed(sf::Mouse::Button button);

    /*!
     * \brief onMouseReleased
     * \param button
     */
    virtual void onMouseReleased(sf::Mouse::Button button);

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
    sf::RectangleShape m_testRect;
    // Internal use vars
    bool m_mouseIn;
    bool m_mouseButtonLeftEmitted;
    bool m_mouseButtonRightEmitted;
    bool m_mouseButtonMiddleEmitted;
    bool m_mouseXButton1Emitted;
    bool m_mouseXButton2Emitted;
    bool m_mouseButtonLeftReleasedEmitted;
    bool m_mouseButtonRightReleasedEmitted;
    bool m_mouseButtonMiddleReleasedEmitted;
    bool m_mouseXButton1ReleasedEmitted;
    bool m_mouseXButton2ReleasedEmitted;
};




#endif // WIDGET_HPP
