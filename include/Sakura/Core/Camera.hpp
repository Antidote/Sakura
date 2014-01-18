// This file is part of Sakura.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura.  If not, see <http://www.gnu.org/licenses/>

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

namespace Sakura
{
namespace Core
{
class Entity;

/*!
 *  \class Camera
 *  \brief Basic camera class used to control the viewport.
 */
class Camera
{
public:
    /*!
     * \brief Camera default constructor
     */
    Camera();

    /*!
     * \brief Camera constructor which specifies the views position, and size
     *
     * \param pos Position in the world
     * \param size Size of the viewport, also specifies the initial world size
     */
    Camera(const sf::Vector2f& pos, const sf::Vector2f& size);

    /*!
     * \brief ~Camera
     */
    virtual ~Camera();

    /*!
     * \brief setWorld Sets the worlds size in pixels
     * \param w width of the world (in pixels)
     * \param h height of thw rold (in pixels)
     */
    void setWorld(int w, int h);

    /*!
     * \brief setWorld Sets the worlds size in pixels
     * \param world size of the world (in pixels)
     */
    void setWorld(const sf::Vector2i& world);
    /*!
     * \brief world Returns the worlds current size (in pixels)
     * \return  sf::Vector2i The current size (in pixels)
     */
    sf::Vector2i world() const;

    /*!
     * \brief setPosition Sets the camera's position (does not ignore the lock target, keep this in mind)
     * \param x The new x position in world space (allowing for half pixels)
     * \param y The new x position in world space (allowing for half pixels)
     */
    void setPosition(float x, float y);

    /*!
     * \brief setPosition Sets the camera's position (does not ignore the lock target, keep this in mind)
     * \param pos The new position in world space (allowing for half pixels)
     */
    void setPosition(const sf::Vector2f& pos);

    /*!
     * \brief position Returns the Views current position in the world, allowing for half pixels
     * \return  sf::Vector2f The current position
     */
    sf::Vector2f position() const;

    /*!
     * \brief setSize Sets the size of the viewport (useful for changes in resolution)
     * \param int width The size of the viewport.
     * \param int height The size of the viewport.
     */
    void setSize(const float w, const float h);
    /*!
     * \brief setSize Sets the size of the viewport (useful for changes in resolution)
     * \param  sf::Vector2f size The size of the viewport, allowing for half pixels
     */
    void setSize(const sf::Vector2f& size);

    /*!
     * \brief size Returns the viewports current size
     * \return  sf::Vector2f The viewports current size, allowing for half pixels
     */
    sf::Vector2f size() const;

    /*!
     * \brief intersects Checks if the viewport intersects with a specified  sf::RectangleShape
     * \param rect The rectangle to check
     * \return True if they intersect, false otherwise
     */
    bool intersects(const sf::FloatRect& rect) const;

    /*!
     * \brief update Updates the camera based on changes in the world, such as the lock targets current position
     */
    void update();

    /*!
     * \brief move Moves the camera relative to it's current position
     * \param x How much to move in the X coordinate, allowing for half pixels
     * \param y How much to move in the Y coordinate, allowing for half pixels
     * \note Useful for cutscenes, but make sure to set the target to NULL if you wish to do so, otherwise
     * the Target will take precedent over the requested action.
     */
    void move(float x, float y);

    /*!
     * \brief view The camera converted into an  sf::View
     * \return  sf::View the viewport
     * \note This is mostly used internally, you can modify this if you wish, but no guarantees are made.
     * Your modifications may be overrided internally, this is NOT a bug, as it's intended behavior.
     */
    sf::View& view();

    /*!
     * \brief lockedOn Returns the current target that the camera is following
     * \return Entity the target entity
     * \note It's not advised to attempt to modify the target anywhere but in it's class, attempting to do so may result in
     * undesired effects.
     */
    Entity* lockedOn() const;

    /*!
     * \brief setLockedOn Sets the target to the specified entity, if you specify a NULL value, the camera will not follow anything.
     * \param entity
     */
    void setLockedOn(Entity* entity);

    sf::FloatRect viewRect() const;

    /*!
     * \brief virtualSize Returns the initial size of the view, useful for maintaining aspect ratio
     * \return sf::Vector2f the intial view size
     */
    sf::Vector2f virtualSize() const;

    /*!
     * \brief aspect Returns the aspect based on  virtualSize
     * \return float The aspect based on  virtualSize
     */
    float aspect() const;

protected:
private:
    sf::Vector2i m_world;
    sf::Vector2f m_pos;
    sf::Vector2f m_size;
    sf::Vector2f m_virtualSize;
    sf::View     m_view;
    Entity*      m_lock;
};

} // Core
} // Sakura

#endif // CAMERA_HPP
