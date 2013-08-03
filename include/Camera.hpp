#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

class Entity;

class Camera
{
public:
    Camera();
    Camera(const sf::Vector2f& pos, const sf::Vector2f& size);

    virtual ~Camera();

    void setWorld(const sf::Vector2i& world);
    sf::Vector2i world() const;

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f position() const;

    void setSize(const sf::Vector2f& size);
    sf::Vector2f size() const;

    void update();

    void move(float x, float y);

    sf::View& view();

    Entity* lockedOn() const;\
    void setLockedOn(Entity* entity);

protected:
private:
    sf::Vector2i m_world;
    sf::Vector2f m_pos;
    sf::Vector2f m_size;
    sf::View     m_view;
    Entity*      m_lock;
};

#endif // CAMERA_HPP
