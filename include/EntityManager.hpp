#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <string>

#include <SFML/System/Mutex.hpp>
class Entity;

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    Entity* entity(const std::string& name);
    std::vector<Entity*> entities() const;

    Entity* player() const;

    void addEntity(Entity* e);
    void removeEntity(Entity* e);
    void update(sf::Time dt);
    void think(sf::Time dt);
    void draw(sf::RenderTarget& rt);

    void shutdown();
protected:
private:
    sf::Mutex m_mutex;
    bool checkNames(const std::string& name);
    std::vector<Entity*> m_entities;
    bool m_playerSpawned;
};

#endif // ENTITYMANAGER_HPP
