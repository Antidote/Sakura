#ifndef LABEL_HPP
#define LABEL_HPP

#include "Widget.hpp"

#include <SFML/Graphics.hpp>

class Label : public Widget
{
public:

    enum Align
    {
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
    Label(const std::string& name, const sf::Font& font, const std::string& text = "", bool visible = true, bool enabled = true);

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
    sf::Text m_text;
    Align m_alignment;
    sf::Vector2f m_oldPosition;
};

#endif // LABEL_HPP
