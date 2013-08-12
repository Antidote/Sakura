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

private:
};

#endif // HUD_HPP
