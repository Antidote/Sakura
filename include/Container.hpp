#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

class Widget;

class Container
{
public:
    /*!
     * \brief Container
     */
    Container();

    virtual ~Container();

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

private:
    std::vector<Widget*> m_children;
};

#endif // CONTAINER_HPP
