#include "Widget.hpp"
#include "Engine.hpp"
#include "Container.hpp"

Widget::Widget(const std::string &name, bool visible, bool enabled)
    : m_name(name),
      m_visible(visible),
      m_enabled(enabled),
      m_color(sf::Color::White),
      m_mouseIn(false),
      m_mouseButtonLeftEmitted(false),
      m_mouseButtonRightEmitted(false),
      m_mouseButtonMiddleEmitted(false),
      m_mouseXButton1Emitted(false),
      m_mouseXButton2Emitted(false),
      m_mouseButtonLeftReleasedEmitted(false),
      m_mouseButtonRightReleasedEmitted(false),
      m_mouseButtonMiddleReleasedEmitted(false),
      m_mouseXButton1ReleasedEmitted(false),
      m_mouseXButton2ReleasedEmitted(false)
{
}

Widget::Widget(const std::string &name, const sf::Vector2f &position, const sf::Vector2f &size)
    : m_name(name),
      m_position(position),
      m_size(size)
{
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
    m_position = position;
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

void Widget::onKeyPressed(sf::Keyboard::Key key)
{
    m_keyPressSignal.Emit(this, key);
}

void Widget::onKeyReleased(sf::Keyboard::Key key)
{
    m_keyReleaseSignal.Emit(this, key);
}

void Widget::onMousePressed(sf::Mouse::Button button)
{
    m_mousePressSignal.Emit(this, button);
}

void Widget::onMouseReleased(sf::Mouse::Button button)
{
    m_mouseReleaseSignal.Emit(this, button);
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
    m_testRect.setPosition(m_position);
    m_testRect.setSize(m_size);

    if (m_owner && m_owner->activeWidget() == this)
    {
        if (sEngineRef().inputManager().keyboard().isAnyKeyPressed())
            onKeyPressed(sEngineRef().inputManager().keyboard().lastPressed());
        if (sEngineRef().inputManager().keyboard().isAnyKeyReleased())
            onKeyReleased(sEngineRef().inputManager().keyboard().lastReleased());
    }

    if (contains((sf::Vector2f)sEngineRef().inputManager().mouse().mouseLocalPosition()))
    {
        if (!m_mouseIn)
        {
            onMouseEnter();
            m_mouseIn = true;
            m_owner->setActiveWidget(this);
        }

        if (sEngineRef().inputManager().mouse().isAnyButtonPressed())
        {
            switch(sEngineRef().inputManager().mouse().lastPressed())
            {
                case sf::Mouse::Left:
                    if (!m_mouseButtonLeftEmitted)
                    {
                        onMousePressed(sf::Mouse::Left);
                        m_mouseButtonLeftEmitted = true;
                    }
                    break;
                case sf::Mouse::Right:
                    if (!m_mouseButtonRightEmitted)
                    {
                        onMousePressed(sf::Mouse::Right);
                        m_mouseButtonRightEmitted = true;
                    }
                    break;
                case sf::Mouse::Middle:
                    if (!m_mouseButtonMiddleEmitted)
                    {
                        onMousePressed(sf::Mouse::Middle);
                        m_mouseButtonMiddleEmitted = true;
                    }
                    break;
                case sf::Mouse::XButton1:
                    if (!m_mouseXButton1Emitted)
                    {
                        onMousePressed(sf::Mouse::XButton1);
                        m_mouseXButton1Emitted = true;
                    }
                    break;
                case sf::Mouse::XButton2:
                    if (!m_mouseXButton2Emitted)
                    {
                        onMousePressed(sf::Mouse::XButton2);
                        m_mouseXButton2Emitted = true;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            m_mouseButtonLeftEmitted = false;
            m_mouseButtonRightEmitted = false;
            m_mouseButtonMiddleEmitted = false;
            m_mouseXButton1Emitted = false;
            m_mouseXButton2Emitted = false;
        }

        if (sEngineRef().inputManager().mouse().wasButtonReleased(sf::Mouse::Left) && !m_mouseButtonLeftReleasedEmitted)
        {
            onMouseReleased(sf::Mouse::Left);
            m_mouseButtonLeftReleasedEmitted = true;
        }
        else
        {
            m_mouseButtonLeftReleasedEmitted = false;
        }

        if (sEngineRef().inputManager().mouse().wasButtonReleased(sf::Mouse::Right) && !m_mouseButtonRightReleasedEmitted)
        {
            onMouseReleased(sf::Mouse::Right);
            m_mouseButtonRightReleasedEmitted = true;
        }
        else
        {
            m_mouseButtonRightReleasedEmitted = false;
        }
        if (sEngineRef().inputManager().mouse().wasButtonReleased(sf::Mouse::Middle) && !m_mouseButtonMiddleReleasedEmitted)
        {
            onMouseReleased(sf::Mouse::Middle);
            m_mouseButtonMiddleReleasedEmitted = true;
        }
        else
        {
            m_mouseButtonMiddleReleasedEmitted = false;
        }

        if (sEngineRef().inputManager().mouse().wasButtonReleased(sf::Mouse::XButton1) && !m_mouseXButton1ReleasedEmitted)
        {
            onMouseReleased(sf::Mouse::XButton1);
            m_mouseXButton1ReleasedEmitted = true;
        }
        else
        {
            m_mouseXButton1ReleasedEmitted = false;
        }
        if (sEngineRef().inputManager().mouse().wasButtonReleased(sf::Mouse::XButton2) && !m_mouseXButton2ReleasedEmitted)
        {
            m_mouseReleaseSignal.Emit(this, sf::Mouse::XButton2);
            m_mouseXButton2ReleasedEmitted = true;
        }
        else
        {
            m_mouseXButton2ReleasedEmitted = false;

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
