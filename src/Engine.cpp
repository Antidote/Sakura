#include "Engine.hpp"

#include "Camera.hpp"
#include "Link.hpp"
#include "Octorok.hpp"
#include "HeartPickup.hpp"
#include "Hud.hpp"
#include <iostream>
#include "Widget.hpp"
#include "Link.hpp"
#include "Octorok.hpp"
#include "HeartPickup.hpp"

Engine::Engine()
    : m_lastTime(sf::seconds(0)),
      m_fps(0),
      m_isClosing(0)
{
}

Engine::~Engine()
{
}

void Engine::initialize(int width, int height, const std::string &title)
{
    m_window.create(sf::VideoMode(width, height), title);

    m_entityManager.addEntity(new Link());
    Octorok* octorok = new Octorok;
    octorok->setPosition(100, 100);
    m_entityManager.addEntity(octorok);


    m_camera = new Camera(sf::Vector2f(0, 0), sf::Vector2f(320, 240));
    m_camera->setWorld(sf::Vector2i(512, 512));
    m_camera->setLockedOn(m_entityManager.player());
    for (int i = 0; i < 20; i++)
    {
        HeartPickup* heart = new HeartPickup;
        heart->setPosition(rand() % m_camera->world().x, rand() % m_camera->world().y);
        m_entityManager.addEntity(heart);
    }
}

int Engine::run()
{
    while(m_window.isOpen())
    {
        sf::Time currentTime = m_clock.restart();
        m_fps = 1.f / currentTime.asSeconds();
        m_lastTime = currentTime;

        if (m_isClosing)
            m_window.close();

        sf::Event event;
        while(m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_isClosing = true;
        }

        m_inputManager.update();
        m_camera->update();
        m_entityManager.think(m_lastTime);
        m_entityManager.update(m_lastTime);
        m_window.setView(m_camera->view());
        m_window.clear(sf::Color::Magenta);
        m_entityManager.draw(m_window);
        m_window.display();
    }

    return 0;
}

Camera *Engine::camera()
{
    return m_camera;
}

EntityManager& Engine::entityManager()
{
    return m_entityManager;
}

ResourceManager& Engine::resourceManger()
{
    return m_resourceManager;
}

InputManager& Engine::inputManager()
{
    return m_inputManager;
}

Engine& Engine::instance()
{
    static Engine engine;
    return engine;
}

sf::RenderWindow& Engine::window()
{
    return m_window;
}
