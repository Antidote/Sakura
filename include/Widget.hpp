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
     * \brief KeyboardSignal
     */
    typedef Gallant::Signal2<Widget*, sf::Keyboard::Key>    KeyboardSignal;

    /*!
     * \brief MouseButtonSignal
     */
    typedef Gallant::Signal2<Widget*, sf::Mouse::Button>    MouseButtonSignal;

    /*!
     * \brief MouseMoveSignal
     */
    typedef Gallant::Signal1<Widget*>  MouseMoveSignal;

    /*!
     * \brief The Group enum
     */
    enum Group
    {
        GroupAll = -1,
        GroupNone,
        GroupSystem,
        GroupGame,
        GroupOther
    };

    /*!
     * \brief Widget
     * \param name
     * \param group
     * \param visible
     * \param enabled
     */
    Widget(const std::string& name, Group group = GroupNone, bool visible = true, bool enabled = true);

    /*!
     * \brief Widget
     * \param name
     * \param group
     * \param position
     * \param size
     */
    Widget(const std::string& name, Group group, const sf::Vector2f& position, const sf::Vector2f& size);
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
     * \param position
     */
    void setPosition(const sf::Vector2f& position);

    /*!
     * \brief position
     * \return
     */
    sf::Vector2f position() const;

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
    KeyboardSignal*    keyRelese();

    /*!
     * \brief mousePressed
     * \return
     */
    MouseButtonSignal* mousePressed();

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
     * \brief onMouseEnter
     */
    virtual void onMouseEnter();

    /*!
     * \brief onMouseLeave
     */
    virtual void onMouseLeave();

    /*!
     * \brief onMouseClick
     */
    virtual void onMouseClick();

    /*!
     * \brief update
     * \param dt
     */
    virtual void update(sf::Time dt);

    /*!
     * \brief draw
     * \param rt
     */
    virtual void draw(sf::RenderTarget& rt)=0;

private:
    Container*        m_owner;
    std::string       m_name;
    Group             m_group;
    bool              m_visible;
    bool              m_enabled;
    sf::Vector2f      m_position;
    sf::Vector2f      m_size;

    KeyboardSignal    m_keyPressSignal;
    KeyboardSignal    m_keyReleaseSignal;
    MouseButtonSignal m_mousePressSignal;
    MouseMoveSignal   m_mouseEnter;
    MouseMoveSignal   m_mouseLeave;


    // Internal use vars
    bool m_mouseIn;
};




#endif // WIDGET_HPP
