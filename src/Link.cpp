#include "Link.hpp"
#include "Engine.hpp"
#include "SoundResource.hpp"
#include "TextureResource.hpp"
#include "Collideable.hpp"
#include <iostream>

Link::Link()
    : Entity("Link", Entity::Player),
      m_velocity(sf::Vector2f(0, 0)),
      m_state(Idle),
      m_blinkTime(sf::milliseconds(2)),
      m_blinkColor(sf::Color(255, 200, 200, 155)),
      m_invincibilityTime(sf::seconds(1.025))
{
    m_hp = 4*3;
    m_takeDamage = true;

    if (!Engine::instance().resourceManger().soundExists("player/wavs/linkhurt"))
        Engine::instance().resourceManger().loadSound("player/wavs/linkhurt", new SoundResource("LTTP_Link_Hurt.wav"));



    if (!Engine::instance().resourceManger().textureExists("player/png/greentunic"))
    {
        Engine::instance().resourceManger().loadTexture("player/png/greentunic", new TextureResource("link.png"));
        m_linkAnimations = Engine::instance().resourceManger().texture("player/png/greentunic");
    }

    setSize(sf::Vector2f(16, 8));

    for (int i = 0; i < (int)FacingSize; i++)
    {
        Animation animation;
        animation.setSpriteSheet(*m_linkAnimations);
        m_animations.push_back(animation);
        for (int x = 0; x < (int)m_linkAnimations->getSize().x; x += 16)
        {
            m_animations[i].addFrame(sf::IntRect(x, (i*24), 16, 24));
        }

    }

    m_sprite.setAnimation(m_animations[m_facing]);
    m_sprite.setFrameTime(sf::seconds(0.075f));
    m_sprite.setLooped(true);
    m_sprite.setOrigin(0, (m_sprite.getLocalBounds().height/2) + 4);
}

void Link::update(sf::Time delta)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_velocity.x -= 4.f*delta.asSeconds();
        m_facing = West;
        m_state = Moving;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_velocity.x += 4.f*delta.asSeconds();
        m_facing = East;
        m_state = Moving;
    }
    else
        m_velocity.x = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_velocity.y -= 4.f*delta.asSeconds();
        m_facing = North;
        m_state = Moving;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_velocity.y += 4.f*delta.asSeconds();
        m_facing = South;
        m_state = Moving;
    }
    else
        m_velocity.y = 0;

    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
       && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
       m_velocity *= .95f;
    }

    if (m_velocity.x > 64.f*delta.asSeconds())
        m_velocity.x = 64.f*delta.asSeconds();
    if (m_velocity.x < -64.f*delta.asSeconds())
        m_velocity.x = -64.f*delta.asSeconds();

    if (m_velocity.y > 64.f*delta.asSeconds())
        m_velocity.y = 64.f*delta.asSeconds();
    if (m_velocity.y < -64.f*delta.asSeconds())
        m_velocity.y = -64.f*delta.asSeconds();

    if (m_lastFacing != m_facing)
    {
        if (m_sprite.isPlaying())
            m_sprite.stop();
    }

    if (m_state != Idle)
    {
        if (!m_sprite.isPlaying())
            m_sprite.setAnimation(m_animations[m_facing]);
        m_sprite.play();
    }
    if (m_velocity == sf::Vector2f(0, 0))
    {
        m_state = Idle;
        m_sprite.stop();
        m_sprite.setFrame(3);
    }

    if (!m_takeDamage && m_blinkTimer.getElapsedTime() > m_blinkTime)
    {
        m_blinkTimer.restart();
        if (m_sprite.color() != m_blinkColor)
            m_sprite.setColor(m_blinkColor);
        else
            m_sprite.setColor(sf::Color::White);
    }

    if (!m_takeDamage && m_invincibilityTimer.getElapsedTime() > m_invincibilityTime)
    {
        m_sprite.setColor(sf::Color::White);
        m_takeDamage = true;
    }

    doCollision();
    m_pos += m_velocity;

    m_sprite.update(delta);
    m_sprite.setPosition(m_pos);
    m_sprite.setOrigin(0, (m_sprite.getLocalBounds().height/2) + 4);
    m_lastFacing = m_facing;
}

void Link::draw(sf::RenderTarget& rt)
{
    rt.draw(m_sprite);
}

void Link::onDamage(Entity *e)
{
    if (m_takeDamage)
    {
        m_takeDamage = false;
        if (Engine::instance().resourceManger().sound("player/wavs/linkhurt"))
            Engine::instance().resourceManger().sound("player/wavs/linkhurt")->play();
        m_invincibilityTimer.restart();
        m_blinkTimer.restart();

        if (e->position().y < position().y)
            m_velocity.y += 8.f;
        else if (e->position().y > position().y)
            m_velocity.y -= 8.f;

        if (position().x < e->position().x)
            m_velocity.x += 8.f;
        else if (position().x > e->position().x)
            m_velocity.x -= 8.f;
    }
}

void Link::onDeath()
{
}

void Link::doCollision()
{
    for(Entity* entity : Engine::instance().entityManager().entities())
    {
        Collideable* collide = dynamic_cast<Collideable*>(entity);
        if (collide)
            collide->collide(this);
    }
}
