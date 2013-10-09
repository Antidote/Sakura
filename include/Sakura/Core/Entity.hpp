#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

namespace Sakura
{
namespace Core
{
/*!
 * \class Entity
 * \brief The Entity class
 *
 * The entity class defines certain attributes commonly used among entities in a game, <b>ALL</b>
 * objects that are used in the game must inherit from this in order to be added to the entity manager.
 *
 */
class Entity
{ 
public:
    /*!
     * \enum Type
     * \brief Specifies the entities type.
     */
    enum Type
    {
        None,   //!< Primarily used for entities that are not drawn, like triggers,
        Player, //!< Used for player entities \warning It is assumed that any entity of this type inherit from Player
                //!< they do not the engine will produce a fatal error
        Enemy,  //!< Used for entities that attack the player, these may be AI or static objects.
        Pickup  //!< Used for entities that give the player something, e.g health
    };

    /*!
     * \brief The Facing enum
     * \todo Move to a more appropriate place
     */
    enum Facing
    {
        North, //!< Used with the entity is facing north
        South, //!< Used with the entity is facing south
        East,  //!< Used with the entity is facing east
        West,  //!< Used with the entity is facing west
        FacingSize
    };

    /*!
     * \brief Creates an instance of an entity based on the name, and type
     * \param name The name of the entity e.g "Player"
     * \param type The type of the entity e.g Player
     */
    Entity(const std::string& name, Type type = None);

    virtual ~Entity();

    /*!
     * \brief Sets the position of the entity relative to the top left corner of the screen
     * \param x The x position
     * \param y The y position
     */
    void setPosition(const float x, const float y);

    /*!
     * \brief Sets the position of the entity relative to the top left corner of the screen
     * \param pos The position
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
     * \brief Returns the bounds of the entity, used to determine if the entity is in the camera's view.<br />
     * It can be used for basic collision if necessary, but it's recommended to use Collideable
     *
     * \return The bounds of the entity
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

    sf::Vector2f m_pos;        //!< The position of the entity in the world, relative to the top left of the screen.
    sf::Vector2f m_size;       //!< The size of the entity, not to be confused with the collision rectangle.
    std::string  m_name;       //!< The name of the entity, every entity is required to have a unique name.
    int          m_id;         //!< The id of the entity, this is assigned internally and cannot be changed.
    Type         m_type;       //!< The type of entity
    Facing       m_facing;     //!< The direction the entity is currently facing (TODO: Move to a more approprate place)
    Facing       m_lastFacing; //!< The direction the entity was facing last frame (TODO: Move to a more approprate place)
    sf::Vector2f m_velocity;   //!< The velocity of the entity

    sf::Text     m_debugInfo;  //!< Used internally to display information relavant to the entity.
};
} // Core
} // Sakura
#endif // ENTITY_HPP
