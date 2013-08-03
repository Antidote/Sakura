#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "InputManager.hpp"
#include "EntityManager.hpp"
#include "ResourceManager.hpp"

class Camera;
class Hud;
class Widget;

class Engine
{
public:
    virtual ~Engine();

    void initialize(int width, int height, const std::string& title);
    int run();


    Camera* camera();

    EntityManager& entityManager();
    ResourceManager& resourceManger();
    InputManager& inputManager();

    static Engine& instance();

    sf::RenderWindow& window();
protected:
private:
    void operator =(const Engine&);
    Engine(const Engine&);
    Engine();

    Camera*          m_camera;

    EntityManager    m_entityManager;
    ResourceManager  m_resourceManager;
    InputManager     m_inputManager;
    sf::RenderWindow m_window;
    sf::Clock        m_clock;
    sf::Time         m_lastTime;
    float            m_fps;
    bool             m_isClosing;
};

#endif // ENGINE_HPP
