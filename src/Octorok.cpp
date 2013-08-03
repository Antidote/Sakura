#include "Octorok.hpp"
#include "Link.hpp"
#include "Engine.hpp"
#include "EntityManager.hpp"
#include "ResourceManager.hpp"
#include "TextureResource.hpp"
#include <iostream>

Octorok::Octorok()
    : Entity("Octorok", Entity::Enemy),
      m_scale(1.f),
      m_thinkTime(sf::seconds(50.f)),
      m_runTime(sf::seconds(2.f))
{
    setSize(16, 8);



    if (!Engine::instance().resourceManger().textureExists("enemies/octorok/body"))
        if (Engine::instance().resourceManger().loadTexture("enemies/octorok/body", new TextureResource("octorok.png")))
            m_sprite.setTexture(*Engine::instance().resourceManger().texture("enemies/octorok/body"));

    m_sprite.setOrigin(0, m_sprite.getLocalBounds().height/2);
}

void Octorok::collide(Entity *entity)
{
    if (bounds().intersects(entity->bounds()))
    {
        Link* link = dynamic_cast<Link*>(entity);
        if (link)
        {
            link->takeHp(1, this);
        }
    }
}

void Octorok::update(sf::Time delta)
{
    if (m_patrolZone.left == 0 && m_patrolZone.top == 0)\
    {
        m_patrolZone = sf::FloatRect(m_pos.x - 50, m_pos.y - 50, 100, 100);
        m_debugShape.setSize(sf::Vector2f(m_patrolZone.width, m_patrolZone.height));
        m_debugShape.setFillColor(sf::Color::Transparent);
        m_debugShape.setOutlineColor(sf::Color::Black);
        m_debugShape.setOutlineThickness(1.f);
        m_debugShape.setPosition(m_patrolZone.left, m_patrolZone.top);
    }

    switch(m_facing)
    {
        case North:
            m_pos.y -= 64.f*delta.asSeconds();
            break;
        case South:
            m_pos.y += 64.f*delta.asSeconds();
            break;
        case East:
            m_pos.x += 64.f*delta.asSeconds();
            break;
        case West:
            m_pos.x -= 64.f*delta.asSeconds();
            break;
    }

    m_sprite.setPosition(m_pos);
}

void Octorok::draw(sf::RenderTarget &rt)
{
    rt.draw(m_debugShape);
    rt.draw(m_sprite);
}

void Octorok::think(sf::Time dt)
{
    m_currentAiTime += dt;

    if (m_currentAiTime > m_thinkTime)
    {
        m_currentAiTime = sf::seconds(0.f);
        m_aiState = Run; //(AiState)(rand() % AiStateSize);
    }

    switch(m_aiState)
    {
        case Run:
            doRun(dt);
            break;
        case Shoot:
            doShoot(dt);
    }

    //sf::Vector2f playerVec = Engine::instance().entityManager().player()->position();

    //move((playerVec - m_pos) * (2.f*dt.asSeconds()));
}

void Octorok::doRun(sf::Time dt)
{
    m_runThinkTime += dt;

    if (!m_patrolZone.contains(m_pos))
    {
        switch (m_facing)
        {
            case North:
                m_facing = South;
                //m_pos.y -= 8.f*dt.asSeconds();
                break;
            case South:
                m_facing = North;
                //m_pos.y += 8.f*dt.asSeconds();
                break;
            case East:
                m_facing = West;
                //m_pos.x -= 8.f*dt.asSeconds();
                break;
            case West:
                m_facing = East;
                //m_pos.x += 8.f*dt.asSeconds();
                break;
        }
    }

    if (m_runThinkTime > m_runTime)
    {
        m_runThinkTime = sf::seconds(0.f);
        m_facing = (Facing)(rand() % FacingSize);
    }
}

void Octorok::doShoot(sf::Time dt)
{

}
