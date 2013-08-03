#include "Widget.hpp"
#include "Engine.hpp"


Widget::Widget(const std::string &name, Widget::Group group, bool visible, bool enabled)
    : m_name(name),
      m_group(group),
      m_visible(visible),
      m_enabled(enabled),
      m_mouseIn(false)
{

}

Widget::Widget(const std::string &name, Widget::Group group, const sf::Vector2f &position, const sf::Vector2f &size)
    : m_name(name),
      m_group(group),
      m_position(position),
      m_size(size)
{
}

Widget::~Widget()
{

}

UIManager*Widget::owner() const
{
    return m_owner;
}

std::string Widget::name() const
{
    return m_name;
}

void Widget::setPosition(const sf::Vector2f& position)
{
    m_position = position;
}

sf::Vector2f Widget::position() const
{
    return m_position;
}

void Widget::setSize(const sf::Vector2f& size)
{
    m_size = size;
}

sf::Vector2f Widget::size() const
{
    return m_size;
}

int Widget::width() const
{
    return m_size.x;
}

int Widget::height() const
{
    return m_size.y;
}

bool Widget::contains(const sf::Vector2f& point)
{
    sf::FloatRect bounds(m_position, m_size);

    return bounds.contains(point);
}

bool Widget::intersects(const sf::FloatRect& rect)
{
    sf::FloatRect bounds(m_position, m_size);

    return bounds.intersects(rect);
}

Widget::KeyboardSignal* Widget::keyPress()
{
    return &m_keyPressSignal;
}

Widget::KeyboardSignal* Widget::keyRelese()
{
    return &m_keyReleaseSignal;
}

Widget::MouseButtonSignal*Widget::mousePressed()
{
    return &m_mousePressSignal;
}

Widget::MouseMoveSignal*Widget::mouseEnter()
{
    return &m_mouseEnter;
}

Widget::MouseMoveSignal*Widget::mouseLeave()
{
    return &m_mouseLeave;
}

void Widget::onMouseEnter()
{
    m_mouseEnter.Emit(this);
}

void Widget::onMouseLeave()
{
    m_mouseLeave.Emit(this);
}

void Widget::onMouseClick()
{
    m_mousePressSignal.Emit(this, Engine::instance().inputManager().mouse().lastPressed());
}

void Widget::update(sf::Time dt)
{
    if (contains((sf::Vector2f)Engine::instance().inputManager().mouse().mouseLocalPosition()) && !m_mouseIn)
    {
        onMouseEnter();
        m_mouseIn = true;
        if (Engine::instance().inputManager().mouse().isAnyButtonPressed())
            onMouseClick();
    }
    else if (m_mouseIn)
    {
        onMouseLeave();
        m_mouseIn = false;
    }
}
