#ifndef LABEL_HPP
#define LABEL_HPP

#include "Widget.hpp"

#include <SFML/Graphics.hpp>

class Label : public Widget
{
public:
    /*!
     * \brief Label
     * \param name
     * \param font
     * \param text
     * \param group
     * \param visible
     * \param enabled
     */
    Label(const std::string& name, const sf::Font& font, const std::string& text = "", Group group = GroupNone, bool visible = true, bool enabled = true);

    /*!
     * \brief setPosition
     * \param position
     */
    void setPosition(const sf::Vector2f &position);

    /*!
     * \brief update
     * \param dt
     */
    void update(sf::Time dt);

    /*!
     * \brief draw
     * \param rt
     */
    void draw(sf::RenderTarget &rt);
private:
    sf::Text m_text;
};

#endif // LABEL_HPP
