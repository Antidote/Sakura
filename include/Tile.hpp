#ifndef TILE_HPP
#define TILE_HPP


#include <SFML/System/Vector2.hpp>
#include <string>
#include <Types.hpp>

class Tile
{
public:
    Tile();

    void setId(Uint32 id);
    Uint32 id() const;

    void setFlippedHor(bool flipped);
    bool flippedHor() const;
    void setFlippedVer(bool flipped);
    bool flippedVer() const;
    void setFlippedDiag(bool flipped);
    bool flippedDiag() const;

    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f position() const;


    void setTileset(int id);
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
