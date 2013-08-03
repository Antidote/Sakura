#include "Hud.hpp"
#include "Link.hpp"
#include "Engine.hpp"
#include "Camera.hpp"
#include <iostream>
#include <sstream>

Hud::Hud()
    : m_player(NULL)
{
    m_font.loadFromFile("arial.ttf");
    ((sf::Texture&)m_font.getTexture(12)).setSmooth(false);
    m_text.setFont(m_font);
    m_text.setCharacterSize(12);
    m_text.setString("---HUD----");
    m_healthText.setFont(m_font);
    m_healthText.setCharacterSize(12);
    m_healthText.setString("Test");
}

void Hud::update()
{
    m_text.setPosition(Engine::instance().camera()->position());

    if (m_player)
    {
        std::stringstream ss;
        ss << "Health: " << m_player->currentHp();
        m_healthText.setString(ss.str());
    }
    m_healthText.setPosition((Engine::instance().camera()->position().x + Engine::instance().camera()->size().x) - m_healthText.getLocalBounds().width - 8, Engine::instance().camera()->position().y);
}

void Hud::draw(sf::RenderTarget& rt)
{
    rt.draw(m_text);
    rt.draw(m_healthText);
}

void Hud::setPlayer(Link *e)
{
    m_player = e;
}
