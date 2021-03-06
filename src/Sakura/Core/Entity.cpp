#include "Sakura/Core/Entity.hpp"
#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/ResourceManager.hpp"
#include "Sakura/Resources/FontResource.hpp"
#include <sstream>
#include <iostream>

namespace Sakura
{
namespace Core
{

Entity::Entity(const std::string& name, Type type)
    : m_name(name),
      m_id(0),
      m_type(type),
      m_facing(North),
      m_lastFacing(North),
      m_velocity(sf::Vector2f(0, 0))
{
    getUniqueName();

    sf::Font* font = sEngineRef().resourceManager().font("fonts/RetGanon.ttf");
    if (font)
    {
        m_debugInfo.setFont(*font);
        m_debugInfo.setCharacterSize(14);
        ((sf::Texture&)m_debugInfo.getFont()->getTexture(14)).setSmooth(false);
    }

    //std::cout << "Entity created: " << m_name << std::endl;
}

Entity::~Entity()
{
}

void Entity::setPosition(const float x, const float y)
{
    setPosition(sf::Vector2f(x, y));
}

void Entity::setPosition(const sf::Vector2f& pos)
{
    m_pos = pos;
}

sf::Vector2f Entity::position() const
{
    return m_pos;
}

void Entity::move(float x, float y)
{
    move(sf::Vector2f(x, y));
}

void Entity::move(const sf::Vector2f &moveToward)
{
    m_pos += moveToward;
}

void Entity::setSize(const float w, const float h)
{
    setSize(sf::Vector2f(w, h));
}

void Entity::setSize(const sf::Vector2f& size)
{
    m_size = size;
}

sf::Vector2f Entity::size() const
{
    return m_size;
}

sf::FloatRect Entity::bounds() const
{
    return sf::FloatRect(m_pos, m_size);
}

std::string Entity::name() const
{
    return m_name;
}

void Entity::setName(const std::string& name)
{
    m_name = name;
}

int Entity::id() const
{
    return m_id;
}

Entity::Type Entity::type() const
{
    return m_type;
}

void Entity::update(sf::Time delta)
{
    UNUSED(delta);
    std::stringstream debugInfo;
    debugInfo << "Name: " << m_name << "\nX: " << m_pos.x << "\nY: " << m_pos.y;
    debugInfo << "\nV.x: " << m_velocity.x << "\nV.y: " << m_velocity.y;

    m_debugInfo.setString(debugInfo.str());
    m_debugInfo.setPosition((m_pos.x + m_size.y/2)- m_debugInfo.getLocalBounds().width/2, m_pos.y - (m_debugInfo.getLocalBounds().height));
}

void Entity::draw(sf::RenderTarget& rt)
{
    rt.draw(m_debugInfo);
}

void Entity::getUniqueName()
{
    static int EntityCount = 0;
    std::stringstream ss;
    if (sEngineRef().entityManager().entity(m_name) != NULL && sEngineRef().entityManager().entity(m_name) != this)
    {
        ss << m_name << "_" << EntityCount++;
        m_id = EntityCount;
        m_name = ss.str();
    }
}
} // Core
} // Sakura
