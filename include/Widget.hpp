#ifndef WIDGET_HPP
#define WIDGET_HPP


#include <Signal.h>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
class UIManager;



class Widget
{
public:
    typedef Gallant::Signal2<Widget*, sf::Keyboard::Key>    KeyboardSignal;
    typedef Gallant::Signal2<Widget*, sf::Mouse::Button>    MouseButtonSignal;
    typedef Gallant::Signal1<Widget*>  MouseMoveSignal;
    enum Group
    {
        GroupAll = -1,
        GroupNone,
        GroupSystem,
        GroupGame,
        GroupOther
    };

    Widget(const std::string& name, Group group = GroupNone, bool visible = true, bool enabled = true);
    Widget(const std::string& name, Group group, const sf::Vector2f& position, const sf::Vector2f& size);
    virtual ~Widget();

    void setOwner(UIManager* owner);
    UIManager* owner() const;

    std::string name() const;
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f position() const;

    void setSize(const sf::Vector2f& size);
    sf::Vector2f size() const;

    int width() const;
    int height() const;

    bool contains(const sf::Vector2f& point);
    bool intersects(const sf::FloatRect& rect);

    KeyboardSignal*    keyPress();
    KeyboardSignal*    keyRelese();
    MouseButtonSignal* mousePressed();
    MouseMoveSignal*   mouseEnter();
    MouseMoveSignal*   mouseLeave();

    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onMouseClick();
    virtual void update(sf::Time dt);
    virtual void draw(sf::RenderTarget& rt)=0;

private:
    UIManager*        m_owner;
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
