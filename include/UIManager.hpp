#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

class Container;

class UIManager
{
public:
    UIManager();
    ~UIManager();

    void addContainer(Container* c);
    void removeContainer(Container* c);

    void update(sf::Time dt);
    void draw(sf::RenderTarget& rt);

private:
    std::vector<Container*> m_containers;
};

#endif // UIMANAGER_HPP
