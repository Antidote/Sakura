#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/ResourceManager.hpp"
#include "Sakura/Core/SSprite.hpp"
#include "Sakura/Core/SSpritePart.hpp"
#include "Sakura/Core/SSpriteFile.hpp"

namespace Sakura
{
namespace Core
{

SSprite::SSprite(SSpriteFile* root)
    : m_root(root),
      m_currentState(0)
{
}

SSprite::SSprite(SSpriteFile* root, const std::string& name, SSprite::Direction dir)
    : m_root(root),
      m_name(name),
      m_direction(dir),
      m_currentState(0)
{
}

SSprite::~SSprite()
{

}

void SSprite::setPosition(const float x, const float y)
{
    setPosition(sf::Vector2f(x, y));
}

void SSprite::setPosition(const sf::Vector2f& pos)
{
    m_position = pos;
}

sf::Vector2f SSprite::position() const
{
    return m_position;
}

void SSprite::setName(const std::string& name)
{
    m_name = name;
}

std::string SSprite::name() const
{
    return m_name;
}

void SSprite::setDirection(const SSprite::Direction dir)
{
    m_direction = dir;
}

SSprite::Direction SSprite::direction() const
{
    return m_direction;
}

void SSprite::addStateId(int id)
{
    if (m_stateIds.size() >= 65536)
        return;

    if (std::find(m_stateIds.begin(), m_stateIds.end(), id) == m_stateIds.end())
        m_stateIds.push_back(id);
}

sf::Texture& SSprite::currentTexture() const
{
    return sEngineRef().resourceManager().texture(m_root->texture(m_currentState)->Filepath);
}

int SSprite::stateId(int index) const
{
    if (index >= (int)m_stateIds.size())
        return -1;

    return m_stateIds[index];
}

void SSprite::setStateIds(std::vector<int> ids)
{
    if (ids.size() == 0)
        return;

    m_stateIds = ids;
}

Uint32 SSprite::stateCount() const
{
    return m_stateIds.size();
}

void SSprite::setCurrentState(const Uint32 id)
{
    m_currentState = id;
}

void SSprite::addPart(SSpritePart* part)
{
    for (SSpritePart* tmp : m_parts)
    {
        if (tmp == part)
            return;
    }
    m_parts.push_back(part);
}

void SSprite::setParts(std::vector<SSpritePart*> parts)
{
    if (parts.size() == 0)
        return;

    if (m_parts.size() > 0)
    {
        for (SSpritePart* part : m_parts)
        {
            delete part;
            part = NULL;
        }
        m_parts.clear();
    }

    m_parts = parts;
}

Uint32 SSprite::partCount() const
{
    return m_parts.size();
}

std::vector<SSpritePart*> SSprite::parts() const
{
    return m_parts;
}

void SSprite::update(const sf::Time& dt)
{
    if (!m_root)
        return;

    for (SSpritePart* part : m_parts)
        part->update(dt);
}

void SSprite::draw(sf::RenderTarget& rt)
{
    if (!m_root)
        return;

    for (SSpritePart* part : m_parts)
    {
        part->draw(rt);
    }
}

}
}
