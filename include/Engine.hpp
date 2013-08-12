#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "InputManager.hpp"
#include "EntityManager.hpp"
#include "ResourceManager.hpp"
#include "UIManager.hpp"
#include "Camera.hpp"
#include "Log.hpp"

class Engine
{
public:
    enum State
    {
        StateSplash,
        StateMenu,
        StateGame
    };

    virtual ~Engine();

    void initialize(int argc, char* argv[], int width, int height, const std::string& title);
    int run();


    Camera& camera();

    EntityManager& entityManager();
    ResourceManager& resourceManager();
    InputManager& inputManager();
    UIManager& uiManager();
    Log& log();
    static Engine& instance();

    sf::RenderWindow& window();

    void loadSplashResources();
    void destroySplashResources();

    void shutdown();
    void doSplashState(sf::Time dt);
    void doMenuState();
    void doGameState();
protected:
private:

    enum Logo
    {
        LogoSFML,
        LogoNintendo,
        LogoWiiking,
        // Add more logos here

        // This must be last
        LogoCount
    };

    void printSysInfo();
    void operator =(const Engine&);
    Engine(const Engine&);
    Engine();
    Log              m_log;
    Camera           m_camera;

    EntityManager    m_entityManager;
    ResourceManager  m_resourceManager;
    InputManager     m_inputManager;

    UIManager        m_uiManager;
    sf::RenderWindow m_window;
    sf::Clock        m_clock;
    sf::Time         m_lastTime;
    sf::Time         m_splashTime;
    sf::Time         m_currentSplashTime;
    sf::Time         m_fadeTime;
    sf::Text         m_fpsString;
    sf::RectangleShape m_fpsBack;
    float            m_fps;
    bool             m_isClosing;
    bool             m_drawWire;
    bool             m_paused;
    sf::Sprite       m_logoSprite;
    State            m_state;
    sf::Uint32       m_currentLogo;
    float            m_fade;
    float            m_rotation;
    bool             m_fadeOut;
};

#endif // ENGINE_HPP
