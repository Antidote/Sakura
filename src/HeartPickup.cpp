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
    if (!Engine::instance().resourceManger().textureExists("items/pickups/heart"))
    {
        if (Engine::instance().resourceManger().loadTexture("items/pickups/heart", new TextureResource("heartpickup.png")))
            m_animation.setSpriteSheet(*Engine::instance().resourceManger().texture("items/pickups/heart"));
    }

    m_animation.addFrame(sf::IntRect(0, 0, 8, 8));
    m_animation.addFrame(sf::IntRect(8, 0, 8, 8));
    m_animation.addFrame(sf::IntRect(16, 0, 8, 8));
    m_sprite.setAnimation(m_animation);

    if (!Engine::instance().resourceManger().soundExists("player/wavs/itempickup"))
        Engine::instance().resourceManger().loadSound("player/wavs/itempickup", new SoundResource("LTTP_Item.wav"));

}

void HeartPickup::update(sf::Time delta)
{
    if (!m_animation.getSpriteSheet())
    {
        if (Engine::instance().resourceManger().loadTexture("items/pickups/heart", new TextureResource("heartpickup.png")))
            m_animation.setSpriteSheet(*Engine::instance().resourceManger().texture("items/pickups/heart"));
    }
    m_sprite.setAnimation(m_animation);
    m_sprite.setPosition(m_pos);
}

void HeartPickup::draw(sf::RenderTarget &rt)
{
    rt.draw(m_sprite);
}

void HeartPickup::collide(Entity *entity)
{
    if (entity->type() == Player && bounds().intersects(entity->bounds()))
    {
        Link* link = dynamic_cast<Link*>(entity);
        if (link)
        {
            link->giveHp(6);
            if (Engine::instance().resourceManger().soundExists("player/wavs/itempickup"))
                Engine::instance().resourceManger().sound("player/wavs/itempickup")->play();

            Engine::instance().entityManager().removeEntity(this);
        }
    }
}
