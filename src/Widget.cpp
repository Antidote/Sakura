#include "Widget.hpp"
#include "Engine.hpp"
#include "Container.hpp"

Widget::Widget(Container* owner, const std::string &name, bool visible, bool enabled)
    : m_owner(owner),
      m_name(name),
      m_visible(visible),
      m_enabled(enabled),
      m_position(0, 0),
      m_size(1, 1),
      m_color(sf::Color::White),
      m_mouseIn(false)
{
    if (owner)
        owner->addWidget(this);
}

Widget::Widget(Container* owner, const std::string &name, const sf::Vector2f &position, const sf::Vector2f &size)
    : m_owner(owner),
      m_name(name),
      m_visible(true),
      m_enabled(true),
      m_position(position),
      m_size(size),
      m_color(sf::Color::White),
      m_mouseIn(false)
{
    if (owner)
        owner->addWidget(this);
}

Widget::~Widget()
{

}

void Widget::setOwner(Container* owner)
{
    m_owner = owner;
}

Container* Widget::owner() const
{
    return m_owner;
}

std::string Widget::name() const
{
    return m_name;
}

void Widget::setPosition(const float x, const float y)
{
    setPosition(sf::Vector2f(x, y));
}

void Widget::setPosition(const sf::Vector2f& position)
{
    m_position = (m_owner ? m_owner->position()  : sf::Vector2f(0,0)) + position;
}

void Widget::move(const float x, const float y)
{
    move(sf::Vector2f(x, y));
}

void Widget::move(const sf::Vector2f& pos)
{
    m_position += pos;
}

sf::Vector2f Widget::position() const
{
    return m_position;
}

void Widget::setSize(const float w, const float h)
{
    setSize(sf::Vector2f(w, h));
}

void Widget::setSize(const sf::Vector2f& size)
{
    m_size = size;
}

sf::Vector2f Widget::size() const
{
    return m_size;
}

void Widget::setColor(const sf::Color& color)
{
    m_color = color;
}

sf::Color Widget::color() const
{
    return m_color;
}

int Widget::width() const
{
    return m_size.x;
}

int Widget::height() const
{
    return m_size.y;
}

void Widget::setVisible(bool visible)
{
    m_visible = visible;
}

bool Widget::visible() const
{
    return m_visible;
}

void Widget::show()
{
    setVisible();
}

void Widget::hide()
{
    setVisible(false);
}

void Widget::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool Widget::enabled() const
{
    return m_enabled;
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

Widget::KeyboardSignal* Widget::keyRelease()
{
    return &m_keyReleaseSignal;
}

Widget::MouseButtonSignal* Widget::mousePressed()
{
    return &m_mousePressSignal;
}

Widget::MouseButtonSignal* Widget::mouseReleased()
{
    return &m_mouseReleaseSignal;
}

Widget::MouseMoveSignal* Widget::mouseEnter()
{
    return &m_mouseEnter;
}

Widget::MouseMoveSignal* Widget::mouseLeave()
{
    return &m_mouseLeave;
}

Widget::ActivatedSignal* Widget::activated()
{
    return &m_activated;
}

Widget::ActivatedSignal* Widget::deactivated()
{
    return &m_deactivated;
}

void Widget::onKeyPressed(sf::Event::KeyEvent keyEvent)
{
    m_keyPressSignal.Emit(this, keyEvent);
}

void Widget::onKeyReleased(sf::Event::KeyEvent keyEvent)
{
    m_keyReleaseSignal.Emit(this, keyEvent);
}

void Widget::onMousePressed(sf::Event::MouseButtonEvent buttonEvent)
{
    m_mousePressSignal.Emit(this, buttonEvent);
}

void Widget::onMouseReleased(sf::Event::MouseButtonEvent buttonEvent)
{
    m_mouseReleaseSignal.Emit(this, buttonEvent);
}

void Widget::onMouseEnter()
{
    m_mouseEnter.Emit(this);
}

void Widget::onMouseLeave()
{
    m_mouseLeave.Emit(this);
}

void Widget::onActivate()
{
    m_activated.Emit(this);
}

void Widget::onDeactivate()
{
    m_deactivated.Emit(this);
}

void Widget::update(sf::Time dt)
{
    UNUSED(dt);

    if (contains((sf::Vector2f)sEngineRef().inputManager().mouse().mouseLocalPosition()))
    {
        if (!m_mouseIn)
        {
            onMouseEnter();
            m_mouseIn = true;
            m_owner->setActiveWidget(this);
        }
    }
    else if (m_mouseIn)
    {
        onMouseLeave();
        m_mouseIn = false;
    }
}

void Widget::draw(sf::RenderTarget& rt)
{
    UNUSED(rt);
}

void Widget::activate()
{
    onActivate();
}

void Widget::deactivate()
{
    onDeactivate();
}

void Widget::handleKeyPress(sf::Event::KeyEvent keyEvent)
{
    if (m_owner && m_owner->activeWidget() == this)
    {
        onKeyPressed(keyEvent);
    }
}

void Widget::handleKeyRelease(sf::Event::KeyEvent keyEvent)
{
    if (m_owner && m_owner->activeWidget() == this)
    {
        onKeyReleased(keyEvent);
    }
}

void Widget::handleMousePress(sf::Event::MouseButtonEvent buttonEvent)
{
    if (m_mouseIn)
        onMousePressed(buttonEvent);
}

void Widget::handleMouseRelease(sf::Event::MouseButtonEvent buttonEvent)
{
    if (m_mouseIn)
        onMouseReleased(buttonEvent);
}
