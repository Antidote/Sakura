#include "Sakura/Core/UIManager.hpp"
#include "Sakura/Gui/Container.hpp"

namespace Sakura
{
namespace Core
{
UIManager::UIManager()
{
}

UIManager::~UIManager()
{
    for(Gui::Container* c : m_containers)
    {
        delete c;
        c = NULL;
    }

    m_containers.clear();
}

void UIManager::addContainer(Gui::Container* c)
{
    for (Gui::Container* child : m_containers)
    {
        if (c == child)
            return;
    }

    m_containers.push_back(c);
}

void UIManager::removeContainer(Gui::Container* c)
{
    int index = -1;
    for (size_t i = 0; i < m_containers.size(); ++i)
    {
        if (m_containers[i] == c)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        delete c;
        c = NULL;
        m_containers[index] = NULL;
        m_containers.erase(m_containers.begin() + index);
    }
}

void UIManager::update(sf::Time dt)
{
    for (Gui::Container* c : m_containers)
        c->update(dt);
}

void UIManager::draw(sf::RenderTarget& rt)
{
    for (Gui::Container* c : m_containers)
        c->draw(rt);
}

void UIManager::handleKeyPress(sf::Event::KeyEvent keyEvent)
{
    for (Gui::Container* c : m_containers)
        c->handleKeyPress(keyEvent);

}

void UIManager::handleKeyRelease(sf::Event::KeyEvent keyEvent)
{
    for (Gui::Container* c : m_containers)
        c->handleKeyRelease(keyEvent);
}

void UIManager::handleMousePress(sf::Event::MouseButtonEvent buttonEvent)
{
    for (Gui::Container* c : m_containers)
        c->handleMousePress(buttonEvent);
}

void UIManager::handleMouseRelease(sf::Event::MouseButtonEvent buttonEvent)
{
    for (Gui::Container* c : m_containers)
        c->handleMouseRelease(buttonEvent);
}
} // Core
} // Sakura
