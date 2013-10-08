#include "Container.hpp"
#include "Widget.hpp"
#include "Engine.hpp"
#include <GL/gl.h>

Container::Container(float x, float y, int width, int height)
    : m_position(sf::Vector2f(x, y)),
      m_activeIndex(0),
      m_activeWidget(NULL)
{
    m_renderTexture.create(width, height);
    m_renderTexture.setSmooth(false);
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

void Container::setPosition(const float x, const float y)
{
    setPosition(sf::Vector2f(x, y));
}

void Container::setPosition(const sf::Vector2f& pos)
{
    m_position = pos;
}

sf::Vector2f Container::position() const
{
    return m_position;
}

void Container::setSize(int w, int h)
{
    m_renderTexture.create(w, h);
    m_renderTexture.setSmooth(false);
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
    if (!w)
        return;

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
    if (!w)
        return;

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
    m_renderTexture.clear(sf::Color::Transparent);
    for (int i = 0; i <= (sEngineRef().config().settingBoolean("r_drawwire", false) ? 1 : 2); i++)
    {
        if (sEngineRef().config().settingBoolean("r_drawwire", false) && i == 1)
        {
            glDisable(GL_TEXTURE_2D);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        for (Widget* w : m_children)
        {
            if (w->visible())
                w->draw(m_renderTexture);
        }

        if (sEngineRef().config().settingBoolean("r_drawwire", false) && i == 1)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_TEXTURE_2D);
        }
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

void Container::handleKeyPress(sf::Event::KeyEvent keyEvent)
{
    switch (keyEvent.code)
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

    for (Widget* w : m_children)
    {
        if (w->enabled())
            w->handleKeyPress(keyEvent);
    }
}

void Container::handleKeyRelease(sf::Event::KeyEvent keyEvent)
{
    for (Widget* w : m_children)
    {
        if (w->enabled())
            w->handleKeyRelease(keyEvent);
    }
}

void Container::handleMousePress(sf::Event::MouseButtonEvent buttonEvent)
{
    for (Widget* w : m_children)
    {
        if (w->enabled())
            w->handleMousePress(buttonEvent);
    }
}

void Container::handleMouseRelease(sf::Event::MouseButtonEvent buttonEvent)
{
    for (Widget* w : m_children)
    {
        if (w->enabled())
            w->handleMouseRelease(buttonEvent);
    }
}
