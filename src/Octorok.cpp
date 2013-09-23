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
      m_thinkTime(sf::seconds(5.f)),
      m_runTime(sf::seconds(4.f)),
      m_outsidePatrol(false)
{
    setSize(16, 8);



    if (!Engine::instance().resourceManager().textureExists("enemies/octorok/body"))
    {
        if (Engine::instance().resourceManager().loadTexture("enemies/octorok/body", new TextureResource("sprites/octorok.png", true)))
            m_sprite.setTexture(*Engine::instance().resourceManager().texture("enemies/octorok/body"));
    }
    else
        m_sprite.setTexture(*Engine::instance().resourceManager().texture("enemies/octorok/body"));

    if (m_sprite.getTexture())
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

void Octorok::update(sf::Time dt)
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

    m_sprite.setPosition(m_pos);

    Entity::update(dt);
}

void Octorok::draw(sf::RenderTarget &rt)
{
    rt.draw(m_debugShape);
    if (m_sprite.getTexture())
        rt.draw(m_sprite);

    rt.draw(m_debugInfo);
    //Entity::draw(rt);
}

void Octorok::think(sf::Time dt)
{
    m_currentAiTime += dt;

    if (m_currentAiTime > m_thinkTime)
    {
        m_currentAiTime = sf::seconds(0.f);
        if (m_aiState == Run)
            m_aiState = (AiState)(rand() % AiStateSize);
        else
            m_aiState = Run;

    }

    switch(m_aiState)
    {
        case Run:
            doRun(dt);
            break;
        case Shoot:
            doShoot(dt);
        default:
            break;
    }

    //sf::Vector2f playerVec = Engine::instance().entityManager().player()->position();

    //move((playerVec - m_pos) * (2.f*dt.asSeconds()));
}

void Octorok::checkFacing()
{
    if (!m_patrolZone.contains(m_pos)&& !m_outsidePatrol)
    {
        m_outsidePatrol = true;

        switch (m_facing)
        {
            case North:
                m_facing = South;
                m_pos.y = m_patrolZone.top;
                break;
            case South:
                m_facing = North;
                m_pos.y = m_patrolZone.top + m_patrolZone.height;
                break;
            case East:
                m_facing = West;
                m_pos.x = m_patrolZone.left + m_patrolZone.width;
                break;
            case West:
                m_facing = East;
                 m_pos.x = m_patrolZone.left;
                break;
            default:
                break;
        }
    }
    else
    {
        m_outsidePatrol = false;
    }
}

void Octorok::doRun(sf::Time dt)
{
    m_runThinkTime += dt;

    checkFacing();

    if (m_runThinkTime > m_runTime)
    {
        m_runThinkTime = sf::seconds(0.f);
        m_facing = (Facing)(rand() % FacingSize);

        checkFacing();
    }

    switch(m_facing)
    {
        case North:
            m_pos.y -= 64.f*dt.asSeconds();
            break;
        case South:
            m_pos.y += 64.f*dt.asSeconds();
            break;
        case East:
            m_pos.x += 64.f*dt.asSeconds();
            break;
        case West:
            m_pos.x -= 64.f*dt.asSeconds();
            break;
        default:
            break;
    }
}

void Octorok::doShoot(sf::Time dt)
{
    UNUSED(dt);
}
