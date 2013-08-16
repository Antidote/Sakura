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
#include "Console.hpp"
#include "Config.hpp"
#include <thread>

// Unused macro
#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif

class RunState;
class Map;

class Engine
{
public:

    static const std::string SAKURA_VERSION;
    enum State
    {
        StateSplash,
        StateMenu,
        StateGame
    };

    virtual ~Engine();

    void initialize(int argc, char* argv[]);
    void restart();
    int run();


    Camera& camera();
    Console& console();
    EntityManager& entityManager();
    ResourceManager& resourceManager();
    InputManager& inputManager();
    UIManager& uiManager();
    Config& config();

    static Engine& instance();

    sf::RenderWindow& window();

    void setClearColor(const sf::Color& color);
    sf::Color clearColor() const;

    void loadSplashResources();
    void destroySplashResources();

    void shutdown();
    void doSplashState(sf::Time dt);
    void doMenuState();
    void doGameState(bool wireframe, int pass);

    void setWireframe(bool mode);
    void toggleFullscreen();

    bool consoleInitialized();
    void quit();

    static std::string version();
    void setFullscreen(bool isFullscreen);

    Map* currentMap() const;
protected:
private:
    std::string stateString();
    enum Logo
    {
        LogoSFML,
        LogoWiiking,
        // Add more logos here

        // This must be last
        LogoCount,
        LogoNintendo,
    };

    RunState*       m_currentState;
    void printSysInfo();
    void operator =(const Engine&);
    Engine(const Engine&);
    Engine();
    Console          m_console;
    Camera           m_camera;
    EntityManager    m_entityManager;
    ResourceManager  m_resourceManager;
    InputManager     m_inputManager;
    Config           m_config;


    UIManager        m_uiManager;

    sf::Vector2u     m_size;
    std::string      m_title;
    sf::RenderWindow m_window;
    sf::View         m_defaultView;
    sf::Clock        m_clock;
    sf::Time         m_lastTime;
    sf::Time         m_splashTime;
    sf::Time         m_currentSplashTime;
    sf::Time         m_fadeTime;
    sf::Text         m_fpsString;
    sf::Text         m_statsString;
    float            m_fps;
    bool             m_paused;
    sf::Sprite       m_logoSprite;
    State            m_state;
    sf::Uint32       m_currentLogo;
    float            m_fade;
    float            m_rotation;
    bool             m_fadeOut;
    bool             m_inputThreadInitialized;
    sf::Color        m_clearColor;

    std::thread      m_inputThread;

    int              m_argc;
    char**           m_argv;

    // State name -> State Object
    // e.g m_states["game"] = new GameState;
    std::unordered_map<std::string, RunState*> m_states;

    sf::RectangleShape m_colShape;
    Map* m_currentMap;
};

#endif // ENGINE_HPP
