#include "Entity.hpp"
#include <sstream>
#include <iostream>


Entity::Entity(const std::string& name, Type type)
    : m_name(name),
      m_id(0),
      m_type(type),
      m_facing(North),
      m_lastFacing(North)
{
    getUniqueName();
    std::cout << "Entity created: " << m_name << std::endl;
}

Entity::~Entity()
{
    std::cout << "Entity destroyed: " << m_name << std::endl;
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
    ((void)delta);
}

void Entity::draw(sf::RenderTarget& rt)
{
    ((void)rt);
}

void Entity::getUniqueName()
{
    static int id = 0;
    std::stringstream ss;
    ss << m_name << "_" << id++;
    m_id = id;
    m_name = ss.str();
}
