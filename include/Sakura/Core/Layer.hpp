#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <Types.hpp>

namespace Sakura
{
namespace Core
{
class Tile;

/*!
 * \brief The Layer class
 */
class Layer
{
public:
    /*!
     * \brief Layer
     */
    Layer();

    ~Layer();

    /*!
     * \brief setVisible
     * \param visible
     */
    void setVisible(bool visible);

    /*!
     * \brief visible
     * \return
     */
    bool visible() const;

    /*!
     * \brief setZOrder
     * \param zOrder
     */
    void setZOrder(Uint32 zOrder);

    /*!
     * \brief zOrder
     * \return
     */
    Uint32 zOrder() const;

    /*!
     * \brief addTile
     * \param tile
     */
    void addTile(Tile* tile);

    /*!
     * \brief tile
     * \param x
     * \param y
     * \return
     */
    Tile* tile(int x, int y);

    /*!
     * \brief tile
     * \param position
     * \return
     */
    Tile* tile(const sf::Vector2f& position);

    /*!
     * \brief setTiles
     * \param tiles
     */
    void setTiles(const std::vector<Tile*>& tiles);
private:
    bool m_visible;
    Uint32 m_zOrder;

    std::vector<Tile*> m_tiles;
};
} // Core
} // Sakura

#endif // LAYER_HPP
