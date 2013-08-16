#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <Types.hpp>

class Tile;

class Layer
{
public:
    Layer();
    ~Layer();
    void setVisible(bool visible);
    bool visible() const;

    void setZOrder(Uint32 zOrder);
    Uint32 zOrder() const;

    void addTile(Tile* tile);
    Tile* tile(int x, int y);
    Tile* tile(const sf::Vector2f& position);
private:
    bool m_visible;
    Uint32 m_zOrder;

    std::vector<Tile*> m_tiles;
};

#endif // LAYER_HPP
