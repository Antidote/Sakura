#include "Container.hpp"
#include "Widget.hpp"
#include "Engine.hpp"

Container::Container(float x, float y, int width, int height)
    : m_position(sf::Vector2f(x, y)),
      m_activeIndex(0),
      m_activeWidget(NULL)
{
    m_renderTexture.create(width, height);
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

sf::Vector2f Container::position() const
{
    return m_position;
}

void Container::setSize(int w, int h)
{
    m_renderTexture.create(w, h);
}

void Container::setSize(sf::Vector2i size)
{
    setSize(size.x, size.y);
}

sf::Vector2i Container::size() const
{
    return (sf::Vector2i)m_renderTexture.getSize();
}

void Container::addWidget(Widget *w)
{
    // Check if the widget is already a child
    for (Widget* c : m_children)
    {
        if (w == c)
        {
            sEngineRef().console().print(Console::Warning, "Widget %s already exists in container", c->name().c_str());
            return;
        }
    }

    if (m_activeWidget == NULL)
    {
        m_activeWidget = w;
        w->activate();
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

        m_activeWidget = m_children[index];
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
    m_renderSprite.setPosition(m_position);

    for (Widget* w : m_children)
    {
        if (w->enabled())
            w->update(dt);
    }
}

void Container::draw(sf::RenderTarget& rt)
{
    m_renderTexture.clear();
    for (Widget* w : m_children)
    {
        if (w->visible())
            w->draw(m_renderTexture);
    }
    m_renderTexture.display();
    m_renderSprite.setTexture(m_renderTexture.getTexture());

    rt.draw(m_renderSprite);
}

void Container::setActiveWidget(Widget* w)
{
    m_activeWidget->deactivate();
    m_activeWidget = w;
    m_activeIndex = -1;
    w->activate();
}

Widget* Container::activeWidget()
{
    return m_activeWidget;
}

void Container::handleInput(sf::Keyboard::Key code, bool alt, bool control, bool shift, bool system)
{
    UNUSED(alt);
    UNUSED(control);
    UNUSED(shift);
    UNUSED(system);

    switch (code)
    {
        case sf::Keyboard::Up:
        {
            m_activeIndex--;
            m_activeWidget->deactivate();
            if (m_activeIndex < 0)
                m_activeIndex = m_children.size() - 1;

            m_activeWidget = m_children[m_activeIndex];
            m_activeWidget->activate();
        }
            break;
        case sf::Keyboard::Down:
        {
            m_activeIndex++;
            m_activeWidget->deactivate();
            if (m_activeIndex > (int)m_children.size() - 1)
                m_activeIndex = 0;

            m_activeWidget = m_children[m_activeIndex];
            m_activeWidget->activate();
        }
            break;
        default:
            break;
    }
}
