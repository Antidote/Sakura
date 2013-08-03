#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity
{
public:
    enum Type
    {
        None,
        Player,
        Enemy,
        Pickup
    };

    enum Facing
    {
        North,
        South,
        East,
        West,
        FacingSize
    };

    Entity(const std::string& name, Type type = None);
    virtual ~Entity();

    void setPosition(const float x, const float y);
    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f position() const;

    void move(float x, float y);
    void move(const sf::Vector2f& moveToward);

    void setSize(const float w, const float h);
    void setSize(const sf::Vector2f& size);
    sf::Vector2f size() const;

    sf::FloatRect bounds() const;
    std::string name() const;
    int id() const;

    Type type() const;

    Facing facing() const;

    virtual void update(sf::Time delta);
    virtual void draw(sf::RenderTarget& rt);

protected:
    // Appends the ID to the name
    void getUniqueName();

    sf::Vector2f m_pos;
    sf::Vector2f m_size;
    std::string  m_name;
    int          m_id;
    Type         m_type;
    Facing       m_facing;
    Facing       m_lastFacing;
    sf::Vector2f m_velocity;
};

#endif // ENTITY_HPP
