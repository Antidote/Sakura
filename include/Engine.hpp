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

    /*!
     * \brief SAKURA_VERSION
     */
    static const std::string SAKURA_VERSION;

    /*!
     * \brief Engine
     */
    Engine();
    virtual ~Engine();

    /*!
     * \brief initialize
     * \param argc
     * \param argv
     */
    virtual void initialize(int argc, char* argv[]);

    /*!
     * \brief restart
     */
    virtual void restart();
    /*!
     * \brief run
     * \return
     */
    virtual int run();


    /*!
     * \brief camera
     * \return
     */
    Camera& camera();

    /*!
     * \brief console
     * \return
     */
    Console& console();

    /*!
     * \brief entityManager
     * \return
     */
    EntityManager& entityManager();

    /*!
     * \brief resourceManager
     * \return
     */
    ResourceManager& resourceManager();

    /*!
     * \brief inputManager
     * \return
     */
    InputManager& inputManager();

    /*!
     * \brief uiManager
     * \return
     */
    UIManager& uiManager();

    /*!
     * \brief config
     * \return
     */
    Config& config();

    /*!
     * \brief instanceRef
     * \return
     */
    static Engine& instanceRef();

    /*!
     * \brief instancePtr
     * \return
     */
    static Engine* instancePtr();

    /*!
     * \brief window
     * \return
     */
    sf::RenderWindow& window();

    /*!
     * \brief setClearColor
     * \param color
     */
    void setClearColor(const sf::Color& color);

    /*!
     * \brief clearColor
     * \return
     */
    sf::Color clearColor() const;

    /*!
     * \brief shutdown
     */
    void shutdown();

    /*!
     * \brief setWireframe
     * \param mode
     */
    void setWireframe(bool mode);

    /*!
     * \brief toggleFullscreen
     */
    void toggleFullscreen();

    /*!
     * \brief consoleInitialized
     * \return
     */
    bool consoleInitialized();

    /*!
     * \brief quit
     */
    void quit();

    /*!
     * \brief version
     * \return
     */
    static std::string version();

    /*!
     * \brief setFullscreen
     * \param isFullscreen
     */
    void setFullscreen(bool isFullscreen);

    /*!
     * \brief setWindowTitle
     * \param title
     */
    void setWindowTitle(const std::string& title);

    /*!
     * \brief windowTitle
     * \return
     */
    std::string windowTitle() const;

    /*!
     * \brief setCurrentState
     * \param state
     */
    void setCurrentState(const std::string& state);

    /*!
     * \brief state
     * \param name
     * \return
     */
    RunState* state(const std::string& name);

    /*!
     * \brief addState
     * \param state
     */
    void addState(RunState* state);

    /*!
     * \brief currentMap
     * \return
     */
    Map* currentMap() const;
protected:
    void printSysInfo();
    void operator =(const Engine&);
    Engine(const Engine&);
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
    bool             m_vsync;
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
    RunState*       m_currentState;
    static Engine*  m_instance;
};

/*!
 * \brief Macro to get a reference to the Engine
 */
#define sEngineRef Engine::instanceRef

/*!
 * \brief Macro to get a pointer to the Engine
 */
#define sEnginePtr Engine::instancePtr

#endif // ENGINE_HPP
