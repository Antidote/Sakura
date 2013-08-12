#include "Container.hpp"
#include "Widget.hpp"

Container::Container()
{
}

Container::~Container()
{
    for(Widget* w : m_children)
    {
        delete w;
        w = NULL;
    }

    m_children.clear();
}

void Container::addWidget(Widget *w)
{
    // Check if the widget is already a child
    for (Widget* c : m_children)
    {
        if (w == c)
            return;
    }

    w->setOwner(this);
    m_children.push_back(w);
}

void Container::removeWidget(const std::string& name)
{
    int index = -1;
    Widget* w = NULL;
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        if (m_children[i]->name() == name)
        {
            index = i;
            w = m_children[i];
            break;
        }
    }

    if (index != -1)
    {
        delete w;
        w = NULL;
        m_children[index] = NULL;
        m_children.erase(m_children.begin() + index);
    }
}

void Container::removeWidget(Widget* w)
{
    int index = -1;
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        if (m_children[i] == w)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        delete w;
        w = NULL;
        m_children[index] = NULL;
        m_children.erase(m_children.begin() + index);
    }
}

void Container::update(sf::Time dt)
{
    for (Widget* w : m_children)
        w->update(dt);
}

void Container::draw(sf::RenderTarget& rt)
{
    for (Widget* w : m_children)
        w->draw(rt);
}
