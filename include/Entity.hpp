#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity
{
public:
    /*!
     * \brief The Type enum
     */
    enum Type
    {
        None,
        Player,
        Enemy,
        Pickup
    };

    /*!
     * \brief The Facing enum
     */
    enum Facing
    {
        North,
        South,
        East,
        West,
        FacingSize
    };

    /*!
     * \brief Entity
     * \param name
     * \param type
     */
    Entity(const std::string& name, Type type = None);

    virtual ~Entity();

    /*!
     * \brief setPosition
     * \param x
     * \param y
     */
    void setPosition(const float x, const float y);

    /*!
     * \brief setPosition
     * \param pos
     */
    void setPosition(const sf::Vector2f& pos);

    /*!
     * \brief position
     * \return
     */
    sf::Vector2f position() const;

    /*!
     * \brief move
     * \param x
     * \param y
     */
    void move(float x, float y);

    /*!
     * \brief move
     * \param moveToward
     */
    void move(const sf::Vector2f& moveToward);

    /*!
     * \brief setSize
     * \param w
     * \param h
     */
    void setSize(const float w, const float h);

    /*!
     * \brief setSize
     * \param size
     */
    void setSize(const sf::Vector2f& size);

    /*!
     * \brief size
     * \return
     */
    sf::Vector2f size() const;

    /*!
     * \brief bounds
     * \return
     */
    sf::FloatRect bounds() const;

    /*!
     * \brief name
     * \return
     */
    std::string name() const;

    /*!
     * \brief setName
     * \param name
     */
    void setName(const std::string& name);

    /*!
     * \brief id
     * \return
     */
    int id() const;

    /*!
     * \brief type
     * \return
     */
    Type type() const;

    /*!
     * \brief facing
     * \return
     */
    Facing facing() const;

    /*!
     * \brief update
     * \param delta
     */
    virtual void update(sf::Time delta);

    /*!
     * \brief draw
     * \param rt
     */
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

    sf::Text     m_debugInfo;
};

#endif // ENTITY_HPP
