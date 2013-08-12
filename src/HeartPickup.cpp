#include "HeartPickup.hpp"
#include "Link.hpp"
#include "Engine.hpp"
#include "EntityManager.hpp"
#include "ResourceManager.hpp"
#include "SoundResource.hpp"
#include "TextureResource.hpp"
#include <iostream>

HeartPickup::HeartPickup()
    : Entity("HeartPickup", Pickup)
{
    setSize(8, 8);
    if (!Engine::instance().resourceManager().textureExists("items/pickups/heart"))
    {
        if (Engine::instance().resourceManager().loadTexture("items/pickups/heart", new TextureResource("sprites/heartpickup.png")))
            m_animation.setSpriteSheet(*Engine::instance().resourceManager().texture("items/pickups/heart"));
    }
    else
    {
        m_animation.setSpriteSheet(Engine::instance().resourceManager().texture("items/pickups/heart"));
    }

    m_animation.addFrame(sf::IntRect(0, 0, 8, 8));
    m_animation.addFrame(sf::IntRect(8, 0, 8, 8));
    m_animation.addFrame(sf::IntRect(16, 0, 8, 8));
    m_sprite.setAnimation(m_animation);

    if (!Engine::instance().resourceManager().soundExists("player/wavs/itempickup"))
        Engine::instance().resourceManager().loadSound("player/wavs/itempickup", new SoundResource("sounds/LTTP_Item.wav"));

}

void HeartPickup::update(sf::Time dt)
{
    if (m_animation.getSpriteSheet())
    {
        m_sprite.setAnimation(m_animation);
        m_sprite.setPosition(m_pos);
    }

    Entity::update(dt);
}

void HeartPickup::draw(sf::RenderTarget &rt)
{
    if (m_sprite.getTexture())
        rt.draw(m_sprite);
    Entity::draw(rt);
}

void HeartPickup::collide(Entity *entity)
{
    if (entity->type() == Player && bounds().intersects(entity->bounds()))
    {
        Link* link = dynamic_cast<Link*>(entity);
        if (link)
        {
            link->giveHp(6);
            Engine::instance().resourceManager().playSound("player/wavs/itempickup");

            Engine::instance().entityManager().removeEntity(this);
        }
    }
}
