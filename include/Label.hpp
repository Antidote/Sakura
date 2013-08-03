#ifndef LABEL_HPP
#define LABEL_HPP

#include "Widget.hpp"

#include <SFML/Graphics.hpp>

class Label : public Widget
{
public:
    Label(const std::string& name, const sf::Font& font, const std::string& text = "", Group group = GroupNone, bool visible = true, bool enabled = true);

    void setPosition(const sf::Vector2f &position);
    void update(sf::Time dt);
    void draw(sf::RenderTarget &rt);
private:
    sf::Text m_text;
};

#endif // LABEL_HPP
