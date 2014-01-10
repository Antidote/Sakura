#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "Sakura/Core/InputManager.hpp"
#include "Sakura/Core/EntityManager.hpp"
#include "Sakura/Core/ResourceManager.hpp"
#include "Sakura/Core/UIManager.hpp"
#include "Sakura/Core/Camera.hpp"
#include "Sakura/Core/Console.hpp"
#include "Sakura/Core/Config.hpp"
#include "Sakura/Core/CVarManager.hpp"

// Unused macro
#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif

/*!
 * \namespace Sakura
 * \brief The Main namespace.
 *
 * The Sakura namespace is meant to contain all of the Sakura Game Engine's namespaces and classes.<br />
 * <div style="color:red; font-weight:bold;">DO NOT PUT ANY CLASSES IN THE MAIN NAMESPACE</div>
 * \sa Sakura::Core
 * \sa Sakura::Gui
 * \sa Sakura::Resources
 * \sa Sakura::Input
 */
namespace Sakura
{
/*!
 * \namespace Core
 * \brief Core contains all runtime specific classes, e.g the Engine, ResourceManager, InputManager, EntityManager, and the UIManager
 * \sa Sakura::Core::Engine
 * \sa Sakura::Core::ResourceManager
 * \sa Sakura::Core::InputManager
 * \sa Sakura::Core::EntityManager
 * \sa Sakura::Core::UIManager
 * \sa Sakura::Core::Console
 */
namespace Core
{
class RunState;
class Map;
/*!
 * \brief The Engine class
 * \ingroup Core
 */
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
    Engine(int argc, char *argv[]);
    virtual ~Engine();

    /*!
     * \brief initialize
     * \param argc
     * \param argv
     */
    virtual bool initialize();

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
     * \deprecated Use CVarManager instead
     */
    Config& config();

    CVarManager& cvarManager();

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
     * \brief gameVersion
     * \return
     */
    virtual std::string gameVersion() const;

    /*!
     * \brief defaultTitle
     * \return
     */
    virtual std::string defaultTitle() const;

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

    /*!
     * \brief handleEvent
     * \param event
     */
    virtual void onEvent(const sf::Event& event);

    /*!
     * \brief beforeUpdate
     */
    virtual void beforeUpdate();

    /*!
     * \brief onUpdate
     */
    virtual void onUpdate();

    /*!
     * \brief afterUpdate
     */
    virtual void afterUpdate();

    /*!
     * \brief beforeDraw
     */
    virtual void beforeDraw();

    /*!
     * \brief onDrawEntities
     */
    virtual void onDrawEntities();

    /*!
     * \brief onDraw
     */
    virtual void onDraw();

    /*!
     * \brief onDrawConsole
     */
    virtual void onDrawConsole();

    /*!
     * \brief afterDraw
     */
    virtual void afterDraw();
protected:
    virtual void parseCommandLine();
    void printSysInfo();
    void operator =(const Engine&);
    Engine(const Engine&);


    int              m_argc;
    char**           m_argv;

    Console          m_console;
    Camera           m_camera;
    EntityManager    m_entityManager;
    ResourceManager  m_resourceManager;
    InputManager     m_inputManager;
    Config           m_config;
    CVarManager      m_cvarManager;


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
    bool             m_fullscreen;
    float            m_fps;
    bool             m_paused;
    sf::Color        m_clearColor;

    // State name -> State Object
    // e.g m_states["game"] = new GameState;
    std::unordered_map<std::string, RunState*> m_states;

    Map* m_currentMap;
    RunState*       m_currentState;
    static Engine*  m_instance;
};

} // Core
} // Sakura
/*!
 * \brief Macro to get a reference to the Engine
 */
#define sEngineRef Sakura::Core::Engine::instanceRef

/*!
 * \brief Macro to get a pointer to the Engine
 */
#define sEnginePtr Sakura::Core::Engine::instancePtr

#endif // ENGINE_HPP
