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

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

namespace Sakura
{
namespace Gui
{
class Widget;

class Container
{
public:
    /*!
     * \brief Container
     */
    Container(float x, float y, int width, int height);

    virtual ~Container();

    /*!
     * \brief setPosition
     * \param x
     * \param y
     */
    void setPosition(const float x, const float y);

    /*!
     * \brief setPosition
     * \param pos
     */
    void setPosition(const sf::Vector2f& pos);

    /*!
     * \brief position
     * \return
     */
    sf::Vector2f position() const;

    /*!
     * \brief setSize
     * \param w
     * \param h
     */
    void setSize(int w, int h);

    /*!
     * \brief setSize
     * \param size
     */
    void setSize(sf::Vector2i size);


    /*!
     * \brief size
     * \return
     */
    sf::Vector2i size() const;

    /*!
     * \brief addWidget
     * \param w
     */
    void addWidget(Widget* w);

    /*!
     * \brief removeWidget
     * \param name
     */
    void removeWidget(const std::string& name);

    /*!
     * \brief removeWidget
     * \param w
     */
    void removeWidget(Widget* w);

    /*!
     * \brief update
     * \param dt
     */
    virtual void update(sf::Time dt);

    /*!
     * \brief draw
     * \param rt
     */
    virtual void draw(sf::RenderTarget& rt);

    void setActiveWidget(Widget* w);
    Widget* activeWidget();

    virtual void handleKeyPress(sf::Event::KeyEvent keyEvent);
    virtual void handleKeyRelease(sf::Event::KeyEvent keyEvent);

    virtual void handleMousePress(sf::Event::MouseButtonEvent buttonEvent);
    virtual void handleMouseRelease(sf::Event::MouseButtonEvent buttonEvent);

private:
    std::vector<Widget*> m_children;
    sf::Vector2f      m_position;
    sf::RenderTexture m_renderTexture;
    sf::Sprite        m_renderSprite;
    int m_activeIndex;
    Widget* m_activeWidget;
};
} // Gui
} // Sakura

#endif // CONTAINER_HPP
