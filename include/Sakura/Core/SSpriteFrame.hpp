#ifndef SSpRITEFRAME_HPP
#define SSpRITEFRAME_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include <Types.hpp>

#include <vector>

namespace Sakura
{
namespace Core
{
class SSpriteFrame
{
public:
    /*!
     * \brief SSpriteFrame
     */
    SSpriteFrame();

    /*!
     * \brief SSpriteFrame
     * \param offX
     * \param offY
     * \param texX
     * \param texY
     * \param width
     * \param height
     * \param frameTime
     * \param flippedH
     * \param flippedV
     */
    SSpriteFrame(float offX, float offY, float texX, float texY, Uint32 width, Uint32 height, float frameTime, bool flippedH = false, bool flippedV = false);

    /*!
     * \brief SSpriteFrame
     * \param frameOff
     * \param texOff
     * \param size
     * \param frameTime
     * \param flippedH
     * \param flippedV
     */
    SSpriteFrame(const sf::Vector2f& frameOff, const sf::Vector2f& texOff, const sf::Vector2u& size, float frameTime, bool flippedH = false, bool flippedV = false);

    /*!
     * \brief setOffset
     * \param x
     * \param y
     */
    void setOffset(float x, float y);

    /*!
     * \brief setOffset
     * \param offset
     */
    void setOffset(const sf::Vector2f& offset);

    /*!
     * \brief offset
     * \return
     */
    sf::Vector2f offset() const;

    /*!
     * \brief setTextureOffset
     * \param x
     * \param y
     */
    void setTextureOffset(float x, float y);

    /*!
     * \brief setTextureOffset
     * \param texOff
     */
    void setTextureOffset(const sf::Vector2f& texOff);

    /*!
     * \brief textureOffset
     * \return
     */
    sf::Vector2f textureOffset() const;

    /*!
     * \brief setSize
     * \param width
     * \param height
     */
    void setSize(Uint32 width, Uint32 height);

    /*!
     * \brief setSize
     * \param size
     */
    void setSize(const sf::Vector2u& size);

    /*!
     * \brief size
     * \return
     */
    sf::Vector2u size() const;

    /*!
     * \brief setFlippedHorizontally
     * \param val
     */
    void setFlippedHorizontally(const bool val);

    /*!
     * \brief flippedHorizontally
     * \return
     */
    bool flippedHorizontally() const;

    /*!
     * \brief setFlippedVertically
     * \param val
     */
    void setFlippedVertically(const bool val);

    /*!
     * \brief flippedVertically
     * \return
     */
    bool flippedVertically() const;

    /*!
     * \brief setFrameTime
     * \param frameTime
     */
    void setFrameTime(float frameTime);

    /*!
     * \brief frameTime
     * \return
     */
    float frameTime() const;

private:
    sf::Vector2f m_offset;
    sf::Vector2f m_textureOffset;
    sf::Vector2u m_size;
    sf::Time     m_frameTime;
    bool         m_flippedH;
    bool         m_flippedV;
};
}
}

#endif // SSpRITEFRAME_HPP
