#include "Label.hpp"

Label::Label(const std::string &name, const sf::Font& font, const std::string &text, Group group, bool visible, bool enabled)
    : Widget(name, group, visible, enabled)
{
    m_text.setFont(font);
    m_text.setString(text);
}

void Label::update(sf::Time dt)
{
    Widget::update(dt);
}

void Label::draw(sf::RenderTarget& rt)
{
    Label::draw(rt);
}


