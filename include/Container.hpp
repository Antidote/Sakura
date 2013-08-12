#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

class Widget;

class Container
{
public:
    Container();
    virtual ~Container();


    void addWidget(Widget* w);
    void removeWidget(const std::string& name);
    void removeWidget(Widget* w);

    virtual void update(sf::Time dt);
    virtual void draw(sf::RenderTarget& rt);

private:
    std::vector<Widget*> m_children;
};

#endif // CONTAINER_HPP
