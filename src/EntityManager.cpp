#include "EntityManager.hpp"
#include "Entity.hpp"
#include "iostream"
#include "AI.hpp"

EntityManager::EntityManager()
    : m_playerSpawned(false)
{
}

EntityManager::~EntityManager()
{
    for(Entity* e : m_entities)
    {
        delete e;
        e = NULL;
    }

    m_entities.clear();
}

void EntityManager::addEntity(Entity* e)
{
    if (checkNames(e->name()))
    {
        std::cout << "Entity with name " << e->name() << " already exists" << std::endl;
        delete e;
        e = NULL;
        return;
    }

    if (e->type() == Entity::Player)
    {
        if (m_playerSpawned)
        {
            std::cout << "Only one player entity may exist in the world" << std::endl;
            delete e;
            e = NULL;
            return;
        }
        m_playerSpawned = true;
    }

    m_entities.push_back(e);
}

void EntityManager::removeEntity(Entity* e)
{
    int index = -1;
    for (size_t i = 0; i < m_entities.size(); ++i)
    {
        if (m_entities[i] == e)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        delete e;
        e = NULL;
        m_entities[index] = NULL;
        m_entities.erase(m_entities.begin() + index);
    }

}

Entity* EntityManager::entity(const std::string& name)
{
    if (m_entities.size() <= 0)
        return NULL;

    for(Entity* e : m_entities)
    {
        if (e->name() == name)
            return e;
    }

    return NULL;
}

// Should probably do collision here and cull out entities
std::vector<Entity*> EntityManager::entities() const
{
    return m_entities;
}

Entity* EntityManager::player() const
{
    for(Entity* e : m_entities)
    {
        if (e)
        {
            if (e->type() == Entity::Player)
                return e;
        }
    }

    return NULL;
}

void EntityManager::update(sf::Time dt)
{
    if (m_entities.size() <= 0)
        return;

    std::vector<Entity*> copy;
    for (Entity* e : m_entities)
        copy.push_back(e);

    for (Entity* e : m_entities)
    {
        if(e)
            e->update(dt);
    }
}

void EntityManager::think(sf::Time dt)
{
    for (Entity* e : m_entities)
    {
        AI* ai = dynamic_cast<AI*>(e);
        if (ai)
            ai->think(dt);
    }
}

bool sort(Entity *a, Entity *b)
{
    return a->position().y < b->position().y;
}

void EntityManager::draw(sf::RenderTarget& rt)
{
    if (m_entities.size() <= 0)
        return;

    std::sort(m_entities.begin(), m_entities.end(), sort);
    for (Entity* e : m_entities)
        e->draw(rt);
}



bool EntityManager::checkNames(const std::string& name)
{
    for(Entity* e : m_entities)
    {
        if (e->name() == name)
            return true;
    }

    return false;
}
