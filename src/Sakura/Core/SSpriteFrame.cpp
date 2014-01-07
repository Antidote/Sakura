#include "Sakura/Core/SSpriteFrame.hpp"
#include "Sakura/Core/SSpritePart.hpp"

namespace Sakura
{
namespace Core
{

SSpriteFrame::SSpriteFrame()
{
}

SSpriteFrame::SSpriteFrame(float offX, float offY, float texX, float texY, Uint32 width, Uint32 height, float frameTime, bool flippedH, bool flippedV)
    : m_offset(sf::Vector2f(offX, offY)),
      m_textureOffset(sf::Vector2f(texX, texY)),
      m_size(sf::Vector2u(width, height)),
      m_frameTime(sf::seconds(frameTime)),
      m_flippedH(flippedH),
      m_flippedV(flippedV)
{
}

SSpriteFrame::SSpriteFrame(const sf::Vector2f& frameOff, const sf::Vector2f& texOff, const sf::Vector2u& size, float frameTime, bool flippedH, bool flippedV)
    : m_offset(frameOff),
      m_textureOffset(texOff),
      m_size(size),
      m_frameTime(sf::seconds(frameTime)),
      m_flippedH(flippedH),
      m_flippedV(flippedV)
{
}

void SSpriteFrame::setOffset(float x, float y)
{
    setOffset(sf::Vector2f(x, y));
}

void SSpriteFrame::setOffset(const sf::Vector2f& offset)
{
    m_offset = offset;
}

sf::Vector2f SSpriteFrame::offset() const
{
    return m_offset;
}

void SSpriteFrame::setTextureOffset(float x, float y)
{
    setTextureOffset(sf::Vector2f(x, y));
}

void SSpriteFrame::setTextureOffset(const sf::Vector2f& texOff)
{
    m_textureOffset = texOff;
}

sf::Vector2f SSpriteFrame::textureOffset() const
{
    return m_textureOffset;
}

void SSpriteFrame::setSize(Uint32 width, Uint32 height)
{
    setSize(sf::Vector2u(width, height));
}

void SSpriteFrame::setSize(const sf::Vector2u& size)
{
    m_size = size;
}

sf::Vector2u SSpriteFrame::size() const
{
    return m_size;
}

void SSpriteFrame::setFlippedHorizontally(const bool val)
{
    m_flippedH = val;
}

bool SSpriteFrame::flippedHorizontally() const
{
    return m_flippedH;
}

void SSpriteFrame::setFlippedVertically(const bool val)
{
    m_flippedV = val;
}

bool SSpriteFrame::flippedVertically() const
{
    return m_flippedV;
}

void SSpriteFrame::setFrameTime(float frameTime)
{
    m_frameTime = sf::seconds(frameTime);
}

float SSpriteFrame::frameTime() const
{
    return m_frameTime.asSeconds();
}
}
}
