#include "Camera.hpp"
#include "Entity.hpp"

Camera::Camera(const sf::Vector2f &pos, const sf::Vector2f &size)
    : m_world(size),
      m_pos(pos),
      m_size(size),
      m_view(pos, size),
      m_lock(NULL)
{
}

Camera::~Camera()
{
}

void Camera::setWorld(const sf::Vector2i &world)
{
    m_world = world;
}

sf::Vector2i Camera::world() const
{
    return m_world;
}

void Camera::setPosition(const sf::Vector2f &pos)
{
    m_pos = pos;
}

sf::Vector2f Camera::position() const
{
    return m_pos;
}

void Camera::setSize(const sf::Vector2f &size)
{
    m_size = size;
}

sf::Vector2f Camera::size() const
{
    return m_size;
}

void Camera::setLockedOn(Entity *entity)
{
    m_lock = entity;
}

Entity* Camera::lockedOn() const
{
    return m_lock;
}

void Camera::update()
{
    if (m_lock)
        m_pos = m_lock->position() - (m_size / 2.f);

    if (m_pos.x < 0)
        m_pos.x = 0;
    if (m_pos.x > m_world.x - m_size.x)
        m_pos.x = m_world.x - m_size.x;

    if (m_pos.y < 0)
        m_pos.y = 0;
    if (m_pos.y > m_world.y - m_size.y)
        m_pos.y = m_world.y - m_size.y;

    m_view = sf::View(sf::FloatRect(m_pos, m_size));
}

void Camera::move(float x, float y)
{
    m_pos += sf::Vector2f(x, y);
}

sf::View& Camera::view()
{
    return m_view;
}
