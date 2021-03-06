#include "Sakura/Core/Camera.hpp"
#include "Sakura/Core/Entity.hpp"

namespace Sakura
{
namespace Core
{
Camera::Camera(const sf::Vector2f &pos, const sf::Vector2f &size)
    : m_world(size),
      m_pos(pos),
      m_size(size),
      m_virtualSize(size), // This should never be changed
      m_view(pos, size),
      m_lock(NULL)
{
}

Camera::~Camera()
{
}

void Camera::setWorld(int w, int h)
{
    setWorld(sf::Vector2i(w, h));
}

void Camera::setWorld(const sf::Vector2i &world)
{
    m_world = world;
}

sf::Vector2i Camera::world() const
{
    return m_world;
}

void Camera::setPosition(float x, float y)
{
    setPosition(sf::Vector2f(x, y));
}

void Camera::setPosition(const sf::Vector2f &pos)
{
    m_pos = pos;
}

sf::Vector2f Camera::position() const
{
    return m_pos;
}

void Camera::setSize(const float w, const float h)
{
    setSize(sf::Vector2f(w, h));
}

void Camera::setSize(const sf::Vector2f &size)
{
    m_size = size;
}

sf::Vector2f Camera::size() const
{
    return m_size;
}

bool Camera::intersects(const sf::FloatRect& rect) const
{
    return viewRect().intersects(rect);
}

void Camera::setLockedOn(Entity *entity)
{
    m_lock = entity;
}

sf::FloatRect Camera::viewRect() const
{
    return sf::FloatRect(m_pos.x, m_pos.y, m_size.x, m_size.y);
}

sf::Vector2f Camera::virtualSize() const
{
    return m_virtualSize;
}

float Camera::aspect() const
{
    return m_virtualSize.x/m_virtualSize.y;
}


Entity* Camera::lockedOn() const
{
    return m_lock;
}

void Camera::update()
{
    if (m_lock)
        m_pos = m_lock->position() - (m_size * .5f);

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

} // Core
} // Sakura
