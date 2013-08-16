#include "Link.hpp"
#include "Engine.hpp"
#include "SoundResource.hpp"
#include "TextureResource.hpp"
#include "Collideable.hpp"
#include "Map.hpp"
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

    if (!Engine::instance().resourceManager().soundExists("player/wavs/linkhurt"))
        Engine::instance().resourceManager().loadSound("player/wavs/linkhurt", new SoundResource("sounds/LTTP_Link_Hurt.wav"));



    if (!Engine::instance().resourceManager().textureExists("player/png/greentunic"))
    {
        Engine::instance().resourceManager().loadTexture("player/png/greentunic", new TextureResource("sprites/link.png", true));
        m_linkAnimations = Engine::instance().resourceManager().texture("player/png/greentunic");
    }
    else
    {
        m_linkAnimations = Engine::instance().resourceManager().texture("player/png/greentunic");

    }

    setSize(sf::Vector2f(8, 8));

    if (m_linkAnimations)
    {
        for (int i = 0; i < (int)FacingSize; i++)
        {
            Animation animation;
            animation.setSpriteSheet(*m_linkAnimations);
            m_animations.push_back(animation);
            for (int x = 0; x < (int)m_linkAnimations->getSize().x; x += 16)
                m_animations[i].addFrame(sf::IntRect(x, (i*24), 16, 24));
        }
    }

    m_sprite.setAnimation(m_animations[m_facing]);
    m_sprite.setFrameTime(sf::seconds(0.075f));
    m_sprite.setLooped(true);
    if (m_animations.size() > 0)
        m_sprite.setOrigin(0, (m_sprite.getLocalBounds().height/2) + 4);
}

void Link::update(sf::Time dt)
{
    if (Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::Left) ||
        Engine::instance().inputManager().joystick().isButtonPressed(0, 7))
    {
        if (m_velocity.x > 0)
            m_velocity.x = 0;
        m_velocity.x -= 4.f*dt.asSeconds();
        m_facing = West;
        m_state = Moving;
    }
    else if (Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::Right) ||
             Engine::instance().inputManager().joystick().isButtonPressed(0, 5))
    {
        if (m_velocity.x < 0)
            m_velocity.x = 0;
        m_velocity.x += 4.f*dt.asSeconds();
        m_facing = East;
        m_state = Moving;
    }
    else
        m_velocity.x = 0;

    if (Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::Up)||
        Engine::instance().inputManager().joystick().isButtonPressed(0, 4))
    {
        m_velocity.y -= 4.f*dt.asSeconds();
        m_facing = North;
        m_state = Moving;
    }
    else if (Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::Down)||
             Engine::instance().inputManager().joystick().isButtonPressed(0, 6))
    {
        m_velocity.y += 4.f*dt.asSeconds();
        m_facing = South;
        m_state = Moving;
    }
    else
        m_velocity.y = 0;

    if(((Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::Left) || Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::Right))
        && (Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::Up) || Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::Down)))
       || ((Engine::instance().inputManager().joystick().isButtonPressed(0, 7) || Engine::instance().inputManager().joystick().isButtonPressed(0, 5))
           && (Engine::instance().inputManager().joystick().isButtonPressed(0, 4) || Engine::instance().inputManager().joystick().isButtonPressed(0, 6))))
    {
        m_velocity *= .95f;
    }

    if (m_velocity.x > 64.f*dt.asSeconds())
        m_velocity.x = 64.f*dt.asSeconds();
    if (m_velocity.x < -64.f*dt.asSeconds())
        m_velocity.x = -64.f*dt.asSeconds();

    if (m_velocity.y > 64.f*dt.asSeconds())
        m_velocity.y = 64.f*dt.asSeconds();
    if (m_velocity.y < -64.f*dt.asSeconds())
        m_velocity.y = -64.f*dt.asSeconds();

    if (m_lastFacing != m_facing)
    {
        if (m_sprite.isPlaying())
            m_sprite.stop();
    }

    if (m_state != Idle)
    {
        if (!m_sprite.isPlaying() && m_animations.size() > 0)
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
    Entity::update(dt);

    m_pos += m_velocity;

    m_sprite.update(dt);

    m_sprite.setPosition(m_pos);

    m_lastFacing = m_facing;

}

void Link::draw(sf::RenderTarget& rt)
{
    Entity::draw(rt);
    if (m_sprite.getTexture())
        rt.draw(m_sprite);
}

void Link::onDamage(Entity *e)
{
    if (m_takeDamage)
    {
        m_takeDamage = false;
        if (Engine::instance().resourceManager().soundExists("player/wavs/linkhurt"))
            Engine::instance().resourceManager().playSound("player/wavs/linkhurt");
        m_invincibilityTimer.restart();
        m_blinkTimer.restart();

        if (e)
        {
            switch(m_facing)
            {
                case North:
                case South:
                {
                    if (e->position().y < position().y)
                        m_velocity.y += 10.f;
                    else if (e->position().y > position().y)
                        m_velocity.y -= 10.f;
                }
                    break;
                case East:
                case West:
                {
                    if (e->position().x < position().x)
                        m_velocity.x += 10.f;
                    else if (e->position().x > position().x)
                        m_velocity.x -= 10.f;
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void Link::onDeath()
{
}

void Link::giveRupees(int num)
{
    if (m_rupees == m_maxRupees)
        return;

    m_rupees += num;
}

void Link::takeRupees(int num)
{
    m_rupees -= num;

    if (m_rupees < 0)
    {
        m_rupees = 0;
    }

}

void Link::doCollision()
{
    int tileW = Engine::instance().currentMap()->tileWidth();
    int tileH = Engine::instance().currentMap()->tileHeight();
    int tile1X = std::floor(((m_pos.x)/tileW));
    int tile1Y = std::floor(((m_pos.y)/tileH));
    int tile2X = std::floor(((m_pos.x + m_size.x)/tileW));
    int tile2Y = std::floor(((m_pos.y + m_size.y)/tileH));
    Cell* cell1 = Engine::instance().currentMap()->collision(tile1X, tile1Y);
    Cell* cell2 = Engine::instance().currentMap()->collision(tile2X, tile2Y);

    sf::IntRect cell1Bounds(sf::Vector2i(tile1X * tileW , tile1Y * tileH), sf::Vector2i(tileW, tileH));
    sf::IntRect cell2Bounds(sf::Vector2i(tile2X * tileW , tile2Y * tileH), sf::Vector2i(tileW, tileH));
    if (cell1)
    {
        if (cell1Bounds.contains(std::abs(m_pos.x - (m_size.x/2)), std::abs(m_pos.y)))
        {
            if (m_velocity.x < 0)
            {
                if ((cell1->CollisionType & ColTypeSolid) == ColTypeSolid)
                {
                    //m_velocity.x = 0;
                    m_pos.x += 2;
                }
            }
            if ((cell1->CollisionType & ColTypeDamage) == ColTypeDamage)
            {
                takeHp((int)cell1->FlippedAndDamage, NULL);
            }
        }
        if (cell1Bounds.contains(std::abs(m_pos.x + (m_size.x/2)), std::abs(m_pos.y)))
        {
            if (m_velocity.x > 0)
            {
                if ((cell1->CollisionType & ColTypeSolid) == ColTypeSolid)
                {
                    //m_velocity.x = 0;
                    m_pos.x -= 2;
                }
            }
            if ((cell1->CollisionType & ColTypeDamage) == ColTypeDamage)
            {
                takeHp((int)cell1->FlippedAndDamage, NULL);
            }
        }

        if (cell1Bounds.contains(std::abs(m_pos.x), std::abs(m_pos.y - (m_size.y/2))))
        {
            if (m_velocity.y < 0)
            {
                if ((cell1->CollisionType & ColTypeSolid) == ColTypeSolid)
                {
                    //m_velocity.y = 0;
                    m_pos.y += 2;
                }
            }
            if ((cell1->CollisionType & ColTypeDamage) == ColTypeDamage)
            {
                takeHp((int)cell1->FlippedAndDamage, NULL);
            }
        }
        if (cell1Bounds.contains(std::abs(m_pos.x), std::abs(m_pos.y + (m_size.y/2))))
        {
            if (m_velocity.y > 0)
            {
                if ((cell1->CollisionType & ColTypeSolid) == ColTypeSolid)
                {
                    //m_velocity.y = 0;
                    m_pos.y -= 2;
                }
            }
            if ((cell1->CollisionType & ColTypeDamage) == ColTypeDamage)
            {
                takeHp((int)cell1->FlippedAndDamage, NULL);
            }
        }
    }



    if (cell2)
    {
        if (cell2Bounds.contains(std::abs(m_pos.x - (m_size.x/2)), std::abs(m_pos.y)))
        {
            if (m_velocity.x < 0)
            {
                if ((cell1->CollisionType & ColTypeSolid) == ColTypeSolid)
                {
                    //m_velocity.x = 0;
                    m_pos.x += 2;
                }
            }

            if ((cell1->CollisionType & ColTypeDamage) == ColTypeDamage)
            {
                takeHp((int)cell1->FlippedAndDamage, NULL);
            }
        }
        if (cell2Bounds.contains(std::abs(m_pos.x + (m_size.x/2)), std::abs(m_pos.y)))
        {
            if (m_velocity.x > 0)
            {
                if ((cell1->CollisionType & ColTypeSolid) == ColTypeSolid)
                {
                    //m_velocity.x = 0;
                    m_pos.x -= 2;
                }
            }

            if ((cell1->CollisionType & ColTypeDamage) == ColTypeDamage)
            {
                takeHp((int)cell1->FlippedAndDamage, NULL);
            }
        }

        if (cell2Bounds.contains(std::abs(m_pos.x), std::abs(m_pos.y - (m_size.y/2))))
        {
            if (m_velocity.y < 0)
            {
                if ((cell1->CollisionType & ColTypeSolid) == ColTypeSolid)
                {
                    //m_velocity.y = 0;
                    m_pos.y += 2;
                }
            }
            if ((cell1->CollisionType & ColTypeDamage) == ColTypeDamage)
            {
                takeHp((int)cell1->FlippedAndDamage, NULL);
            }
        }
        if (cell2Bounds.contains(std::abs(m_pos.x), std::abs(m_pos.y + (m_size.y/2))))
        {
            if (m_velocity.y > 0)
            {
                if ((cell1->CollisionType & ColTypeSolid) == ColTypeSolid)
                {
                    //m_velocity.y = 0;
                    m_pos.y -= 2;
                }
            }

            if ((cell1->CollisionType & ColTypeDamage) == ColTypeDamage)
            {
                takeHp((int)cell1->FlippedAndDamage, NULL);
            }
        }
    }

    for(Entity* entity : Engine::instance().entityManager().entities())
    {
        Collideable* collide = dynamic_cast<Collideable*>(entity);
        if (collide)
            collide->collide(this);
    }
}
