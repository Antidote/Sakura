#include "Sakura/Gui/Label.hpp"
#include "Sakura/Gui/Container.hpp"

namespace Sakura
{
namespace Gui
{
Label::Label(Container* owner, const std::string &name, const sf::Font& font, const std::string &text, bool visible, bool enabled)
    : Widget(owner, name, visible, enabled),
      m_alignment(None)
{
    m_text.setFont(font);
    m_text.setString(text);
    updateLabel();
    // reset the position for the next frame.
    m_position = m_oldPosition;
}

void Label::update(sf::Time dt)
{
    updateLabel();
    Widget::update(dt);
    // reset the position for the next frame.
    m_position = m_oldPosition;
}

void Label::draw(sf::RenderTarget& rt)
{
    rt.draw(m_text);
    Widget::draw(rt);
}

void Label::setAlignment(Label::Align align)
{
    m_alignment = align;
}

Label::Align Label::alignment() const
{
    return m_alignment;
}

void Label::setFontSize(int fontSize)
{
    m_text.setCharacterSize(fontSize);
}

int Label::fontSize() const
{
    return m_text.getCharacterSize();
}

void Label::updateLabel()
{
    m_oldPosition = m_position;
    m_size = sf::Vector2f(m_text.getLocalBounds().width, m_text.getLocalBounds().height*2);
    m_text.setColor(m_color);
    switch(m_alignment)
    {
        case Left:
            m_position.x = 0;
            break;
        case Right:
            m_position.x = (m_owner->size().x - m_size.x);
            break;
        case Center:
            m_position.x = (m_owner->size().x *.5f) - (m_size.x * .5f);
            break;
        default:
            break;
    }

    m_text.setPosition(m_position);
}
} // Gui
} // Sakura

