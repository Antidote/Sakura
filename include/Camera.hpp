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

    void setWorld(int w, int h);
    void setWorld(const sf::Vector2i& world);
    sf::Vector2i world() const;

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f position() const;

    void setSize(const sf::Vector2f& size);
    sf::Vector2f size() const;

    bool intersects(const sf::FloatRect& rect) const;
    void update();

    void move(float x, float y);

    sf::View& view();

    Entity* lockedOn() const;\
    void setLockedOn(Entity* entity);

    sf::FloatRect viewRect() const;

    sf::Vector2f virtualSize() const;

    float aspect() const;

protected:
private:
    sf::Vector2i m_world;
    sf::Vector2f m_pos;
    sf::Vector2f m_size;
    sf::Vector2f m_virtualSize;
    sf::View     m_view;
    Entity*      m_lock;
};

#endif // CAMERA_HPP
