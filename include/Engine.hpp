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

    virtual void initialize(int argc, char* argv[]);
    virtual void restart();
    virtual int run();


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

    void shutdown();

    void setWireframe(bool mode);
    void toggleFullscreen();

    bool consoleInitialized();
    void quit();

    static std::string version();
    void setFullscreen(bool isFullscreen);

    void setCurrentState(const std::string& state);
    RunState* state(const std::string& state);
    Map* currentMap() const;
protected:
private:
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
    sf::Time         m_fadeTime;
    sf::Text         m_fpsString;
    sf::Text         m_statsString;
    int              m_frameLimit;
    float            m_fps;
    bool             m_paused;
    bool             m_inputThreadInitialized;
    sf::Color        m_clearColor;

    std::thread      m_inputThread;

    int              m_argc;
    char**           m_argv;

    // State name -> State Object
    // e.g m_states["game"] = new GameState;
    std::unordered_map<std::string, RunState*> m_states;

    Map* m_currentMap;
};

#endif // ENGINE_HPP
