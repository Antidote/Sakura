#ifndef TILE_HPP
#define TILE_HPP


#include <SFML/System/Vector2.hpp>
#include <string>
#include <Types.hpp>

class Tile
{
public:
    /*!
     * \brief Tile
     */
    Tile();

    /*!
     * \brief setId
     * \param id
     */
    void setId(Uint32 id);

    /*!
     * \brief id
     * \return
     */
    Uint32 id() const;

    /*!
     * \brief setFlippedHor
     * \param flipped
     */
    void setFlippedHor(bool flipped);

    /*!
     * \brief flippedHor
     * \return
     */
    bool flippedHor() const;

    /*!
     * \brief setFlippedVer
     * \param flipped
     */
    void setFlippedVer(bool flipped);

    /*!
     * \brief flippedVer
     * \return
     */
    bool flippedVer() const;

    /*!
     * \brief setFlippedDiag
     * \param flipped
     */
    void setFlippedDiag(bool flipped);

    /*!
     * \brief flippedDiag
     * \return
     */
    bool flippedDiag() const;

    /*!
     * \brief setPosition
     * \param x
     * \param y
     */
    void setPosition(float x, float y);

    /*!
     * \brief setPosition
     * \param position
     */
    void setPosition(const sf::Vector2f& position);

    /*!
     * \brief position
     * \return
     */
    sf::Vector2f position() const;


    /*!
     * \brief setTileset
     * \param id
     */
    void setTileset(int id);

    /*!
     * \brief tileset
     * \return
     */
    Uint32 tileset() const;

private:
    Uint32       m_id;
    Uint32       m_tileset;
    bool         m_flippedHor;
    bool         m_flippedVer;
    bool         m_flippedDiag;
    sf::Vector2f m_position;
};

#endif // TILE_HPP
