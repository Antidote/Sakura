#ifndef LABEL_HPP
#define LABEL_HPP

#include "Widget.hpp"

#include <SFML/Graphics.hpp>

namespace Sakura
{
namespace Gui
{
class Label : public Widget
{
public:

    enum Align
    {
        None = -1,
        Left,
        Right,
        Center
    };

    /*!
     * \brief Label
     * \param name
     * \param font
     * \param text
     * \param visible
     * \param enabled
     */
    Label(Container* owner, const std::string& name, const sf::Font& font, const std::string& text = "", bool visible = true, bool enabled = true);

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


    void setAlignment(Align align);
    Align alignment() const;

    void setFontSize(int fontSize);
    int fontSize() const;
private:
    void updateLabel();
    sf::Text m_text;
    Align m_alignment;
    sf::Vector2f m_oldPosition;
};
} // Gui
} // Sakura

#endif // LABEL_HPP
