#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>

class Link;

class Hud
{
public:
    Hud();

    void update();
    void draw(sf::RenderTarget& rt);

    void setPlayer(Link* e);

private:
    Link*      m_player;
    sf::Text   m_text;
    sf::Text   m_healthText;
    sf::Font   m_font;
    sf::String m_string;
};

#endif // HUD_HPP
