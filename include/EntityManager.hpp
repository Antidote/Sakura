#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <string>

#include <SFML/System/Mutex.hpp>
class Entity;
class Player;
class Console;

class EntityManager
{
public:
    /*!
     * \brief EntityManager
     */
    EntityManager();

    ~EntityManager();

    /*!
     * \brief entity
     * \param name
     * \return
     */
    Entity* entity(const std::string& name);

    /*!
     * \brief entities
     * \return
     */
    std::vector<Entity*> entities() const;

    /*!
     * \brief player
     * \param id
     * \return
     */
    Player* player(int id) const;

    /*!
     * \brief addEntity
     * \param e
     */
    void addEntity(Entity* e);
    /*!
     * \brief removeEntity
     * \param e
     */
    void removeEntity(Entity* e);
    /*!
     * \brief update
     * \param dt
     */
    void update(sf::Time dt);

    /*!
     * \brief think
     * \param dt
     */
    void think(sf::Time dt);

    /*!
     * \brief draw
     * \param rt
     */
    void draw(sf::RenderTarget& rt);

    /*!
     * \brief shutdown
     */
    void shutdown();
protected:
private:
    sf::Mutex m_mutex;
    bool checkNames(const std::string& name);
    std::vector<Entity*> m_entities;
    bool m_playerSpawned;
};

#endif // ENTITYMANAGER_HPP
