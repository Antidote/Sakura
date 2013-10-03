#include "EntityManager.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Engine.hpp"
#include "Camera.hpp"
#include "iostream"
#include "AI.hpp"

EntityManager::EntityManager()
    : m_playerSpawned(false)
{
}

EntityManager::~EntityManager()
{
    // Clear any stragglers
    shutdown();
}

void EntityManager::addEntity(Entity* e)
{
    if (checkNames(e->name()))
    {
        sEngineRef().console().print(Console::Warning, "Entity with name %s already exists\n", e->name().c_str());
        delete e;
        e = NULL;
        return;
    }

    if (e->type() == Entity::Player)
    {
        sEngineRef().console().print(Console::Info, "Spawned player with name %s\n", e->name().c_str());
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
        m_entities.erase(m_entities.begin() + index);
        delete e;
        e = NULL;
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

std::vector<Entity*> EntityManager::entities() const
{
    return m_entities;
}

Player* EntityManager::player(int id) const
{
    if (id < 1)
    {
        sEngineRef().console().print(Console::Warning, "Invalid player id %i", id);
        return NULL;
    }

    for(Entity* e : m_entities)
    {
        if (e)
        {
            if (e->type() == Entity::Player)
            {
                Player* p = dynamic_cast<Player*>(e);
                if (!p)
                {
                    sEngineRef().console().print(Console::Fatal, "Entity claims to be player but dynamic_cast failed");
                    break;
                }
                else if (p->playerId() == id)
                {
                    return p;
                }
            }
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
        if (!sEngineRef().camera().intersects(e->bounds()) && e->type() != Entity::Player)
            continue;
        e->update(dt);
    }
}

void EntityManager::think(sf::Time dt)
{
    for (Entity* e : m_entities)
    {
        if (!sEngineRef().camera().intersects(e->bounds()))
            continue;
        AI* ai = dynamic_cast<AI*>(e);
        // clip all entites that aren't in view
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
    {
        // clip all entites that aren't in view
        if (!sEngineRef().camera().intersects(e->bounds()) && e->type() != Entity::Player)
            continue;
        e->draw(rt);
    }
}

void EntityManager::shutdown()
{
    if (m_entities.size() > 0)
    {
        sEngineRef().console().print(Console::Info, "Destroying entities...\n");
        for(Entity* e : m_entities)
        {
            if (e)
            {
                delete e;
                e = NULL;
            }
        }

    }

    m_entities.clear();
    m_playerSpawned = false;
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
