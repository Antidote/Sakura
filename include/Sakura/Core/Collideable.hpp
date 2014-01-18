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

#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

#include <SFML/Graphics.hpp>

namespace Sakura
{
namespace Core
{
class Entity;

/*!
 *  \class Collideable
 *  \brief BaseClass inherited by collideable objects
 */
class Collideable
{
public:
    /*!
     * \brief Called when two entities collide.
     * \param entity The colliding entity
     */
    virtual void collide(Entity* entity)=0;

    /*!
     * \brief Returns the collideable's collision rectangle
     * \return sf::IntRect The collision rectangle
     */
    virtual sf::IntRect collisionRect() const=0;
};
} // Core
} // Sakura

#endif // COLLIDABLE_HPP
