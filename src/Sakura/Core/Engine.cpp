#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/RunState.hpp"
#include "Sakura/Core/Player.hpp"
#include "Sakura/Core/CVar.hpp"
#include "x86cpuinfo.h"
#include "memorysize.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Config.hpp>
#include <iostream>
#include <iomanip>
#include <utility.hpp>
#include <sstream>
#include <AL/al.h>
#include <AL/alc.h>
#include <mutex>


namespace Sakura
{
namespace Core
{

Engine* Engine::m_instance = NULL;
const std::string Engine::SAKURA_VERSION = "0.5a";

Engine::Engine(int argc, char* argv[])
    : m_argc(argc),
      m_argv(argv),
      m_console("log.txt"),
      m_camera(sf::Vector2f(0, 0), sf::Vector2f(512, 224)),
      m_lastTime(sf::seconds(0)),
      m_frameLimit(0),
      m_vsync(false),
      m_fullscreen(false),
      m_fps(0),
      m_paused(false),
      m_clearColor(sf::Color::Black),
      m_currentMap(NULL),
      m_currentState(NULL)
{
    m_instance = this;
    console().print(Console::Info, "Sakura Engine " + version() + ": Initializing");
    console().print(Console::Info, "Built with SFML %i.%i", SFML_VERSION_MAJOR , SFML_VERSION_MINOR);
    console().print(Console::Info, "Build date %s %s", __DATE__, __TIME__);
    // This is a hack to initialize the openALContext
    sf::Sound snd;
    UNUSED(snd);
}

Engine::~Engine()
{
    console().print(Console::Info, "Shutdown complete");
    console().print(Console::Message, "********************** END OF LOG **********************");
}

bool Engine::initialize()
{
    config().initialize("settings.cfg");

    m_title = config().settingLiteral("sys_title", defaultTitle());
    m_size = sf::Vector2u(config().settingInt("vid_width", 640), config().settingInt("vid_height", 480));
    console().print(Console::Info, "Creating context...");
    m_fullscreen = config().settingBoolean("r_fullscreen", false);
    setFullscreen(m_fullscreen);

    m_frameLimit = config().settingInt("sys_framelimit", 60);
    window().setFramerateLimit(m_frameLimit);

    m_vsync = config().settingBoolean("sys_vsync", true);
    window().setVerticalSyncEnabled(m_vsync);

    // QUICK GRAB THE VIEW!!!
    m_defaultView = window().getDefaultView();

    console().print(Console::Info, "Polling Hardware...");
    printSysInfo();
    console().print(Console::Info, "End Hardware poll");
    // Initailize the ResourceManager
    if (!resourceManager().initialize(m_argv[0]))
    {
        // Kill the engine
        window().close();
        return false;
    }

    resourceManager().loadFont("fonts/debug.ttf", true);

    m_clearColor = config().settingColor("r_clearcolor", sf::Color::Black);

    // Initialize the console
    m_console.initialize();


    if (resourceManager().fontExists("fonts/debug.ttf"))
    {
        m_fpsString.setFont(*resourceManager().font("fonts/debug.ttf"));
        m_fpsString.setCharacterSize(20);
        m_fpsString.setColor(sf::Color::White);
        ((sf::Texture&)m_fpsString.getFont()->getTexture(20)).setSmooth(false);
        m_statsString.setFont(*resourceManager().font("fonts/debug.ttf"));
        m_statsString.setCharacterSize(20);
        m_statsString.setColor(sf::Color::White);
        ((sf::Texture&)m_statsString.getFont()->getTexture(20)).setSmooth(false);
    }

    return true;
}

void Engine::restart()
{
    // This function is a bit of a hack,
    // It checks for fullscreen and goes back to windowed
    // if it was. Then it shuts everything down and runs intialize
    // again
    bool wasFullscreen = config().settingBoolean("r_fullscreen", false);
    if (wasFullscreen)
        setFullscreen(false);

    shutdown();
    initialize();

    if (wasFullscreen)
        setFullscreen(true);
}

int Engine::run()
{
    // Only run check if the engine was able to successfully initialize
    // the resource manager
    if (window().isOpen() && (!m_currentState && m_states.size() <= 0))
    {
        console().print(Console::Fatal, "No states specified!!!");
    }

    console().print(Console::Info, "Entering main loop...");
    while(window().isOpen())
    {
        // Check to see if the framelimit has been changed
        if (config().settingInt("sys_framelimit", 60) != m_frameLimit)
        {
            // It has? Let's update the window.
            m_frameLimit = config().settingInt("sys_framelimit", 60);
            window().setFramerateLimit(m_frameLimit);
        }

        // Check to see if vsync has been changed
        if (config().settingBoolean("sys_vsync", true) != m_vsync)
        {
            // It has? Let's update the window
            m_vsync = config().settingBoolean("sys_vsync", true);
            window().setVerticalSyncEnabled(m_vsync);
        }

        if (config().settingBoolean("r_fullscreen", false) != m_fullscreen)
            setFullscreen(config().settingBoolean("r_fullscreen", false));

        m_lastTime = m_clock.restart();
        m_fps = 1.f / m_lastTime.asSeconds();

        std::stringstream fpsStr;
        fpsStr << std::setprecision(4) << "FPS: " << m_fps;
        m_fpsString.setString(fpsStr.str());

        sf::Event event;
        while(m_window.pollEvent(event))
            onEvent(event);

        beforeUpdate();
        onUpdate();
        afterUpdate();

        beforeDraw();
        for (int i = 0; i < (config().settingBoolean("r_drawwire", false) ? 2 : 1); i++)
        {
            window().setView(m_camera.view());
            if (config().settingBoolean("r_drawwire", false) && i == 1)
            {
                glDisable(GL_TEXTURE_2D);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }

            // The reason the draws are separated the way they are is so
            // a developer can hook in at a specific spot in the draw event

            // onDrawEntities allows the developer to draw specifings, either
            // before or after the entities
            onDrawEntities();

            // Reset the view to the default
            // Draw the console over everything
            window().setView(window().getDefaultView());

            // onDraw renders the current state as well as the ui components, the reason for this is simple
            // If anyone wants to draw anything before the ui, they simply need to use the RunState's draw event
            onDraw();

            // onDrawConsole allows the developer to draw things before or after the console
            // by default it draws stats, followed by the console and finally the FPS
            onDrawConsole();

            if (config().settingBoolean("r_drawwire", false) && i == 1)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEnable(GL_TEXTURE_2D);
            }
        }
        afterDraw();
        window().display();

    }
    console().print(Console::Info, "Quit main loop performing preshutdown maintenance...");
    shutdown();
    return 0;
}

Camera& Engine::camera()
{
    return m_camera;
}

EntityManager& Engine::entityManager()
{
    return m_entityManager;
}

ResourceManager& Engine::resourceManager()
{
    return m_resourceManager;
}

InputManager& Engine::inputManager()
{
    return m_inputManager;
}

UIManager& Engine::uiManager()
{
    return m_uiManager;
}

Config& Engine::config()
{
    return m_config;
}

CVarManager& Engine::cvarManager()
{
    return m_cvarManager;
}

Console& Engine::console()
{
    return m_console;
}

Engine& Engine::instanceRef()
{
    return *m_instance;
}

Engine* Engine::instancePtr()
{
    return m_instance;
}

sf::RenderWindow& Engine::window()
{
    return m_window;
}

void Engine::setClearColor(const sf::Color& color)
{
    config().setSettingColor("r_clearcolor", color);
    m_clearColor = color;
}

sf::Color Engine::clearColor() const
{
    return m_clearColor;
}

void Engine::shutdown()
{
    if (window().isOpen())
        window().close();


    console().print(Console::Info, "Clearing states");
    for (std::pair<std::string, RunState*> pair : m_states)
    {
        delete pair.second;
        pair.second = NULL;
    }
    m_states.clear();
    m_currentState = NULL;

    console().print(Console::Info, "Killing Entity Manager...");
    entityManager().shutdown();
    console().print(Console::Info, "Killing Resource Manager...");
    resourceManager().shutdown();
    console().print(Console::Info, "Maintenance complete...");
    console().shutdown();
    config().shutdown();
    cvarManager().writeToFile();
}

void Engine::setWireframe(bool mode)
{
    config().setSettingBoolean("r_drawwire", mode);
}

void Engine::setFullscreen(bool isFullscreen)
{
    // Don't toggle if it's the same
    if (isFullscreen == m_fullscreen && window().isOpen())
        return;

    window().close();
    m_fullscreen = isFullscreen;
    config().setSettingBoolean("r_fullscreen", isFullscreen);
    if (isFullscreen)
        window().create(sf::VideoMode(m_size.x, m_size.y), m_title, sf::Style::Fullscreen);
    else
        window().create(sf::VideoMode(m_size.x, m_size.y), m_title, sf::Style::Titlebar | sf::Style::Close);
}

void Engine::setWindowTitle(const std::string& title)
{
    config().setSettingLiteral("sys_title", title);
    window().setTitle(title);
}

void Engine::setCurrentState(const std::string& state)
{
    if (m_states.find(state) == m_states.end())
    {
        console().print(Console::Warning, "No such state %s", state.c_str());
        return;
    }

    m_currentState = m_states[state];
}

RunState* Engine::state(const std::string& name)
{
    if (m_states.find(name) == m_states.end())
    {
        console().print(Console::Warning, "No such state %s", name.c_str());
        return NULL;
    }

    return m_states[name];
}

void Engine::addState(RunState* newState)
{
    if (m_states.find(newState->name()) != m_states.end())
    {
        console().print(Console::Warning, "State %s already exists\n", newState->name().c_str());
        return;
    }

    m_states[newState->name()] = newState;

    // If there is no current state specified
    // go ahead and assign it to the new state
    if (m_currentState == NULL)
        m_currentState = newState;
}

Map* Engine::currentMap() const
{
    return m_currentMap;
}

void Engine::onEvent(const sf::Event& event)
{
    // Default event handler
    if (event.type == sf::Event::Closed)
        window().close();
    if (event.type == sf::Event::GainedFocus)
        m_paused = false;
    if (event.type == sf::Event::LostFocus)
        m_paused = true;
    if (event.type == sf::Event::TextEntered)
        m_console.handleText(event.text.unicode);
    if (event.type == sf::Event::KeyPressed)
    {
        m_console.handleInput(event.key.code, event.key.alt, event.key.control, event.key.shift, event.key.system);
        if (!console().isOpen() && event.key.code != sf::Keyboard::Unknown)
            uiManager().handleKeyPress(event.key);
    }
    if (event.type == sf::Event::KeyReleased)
    {
        if (!console().isOpen() && event.key.code != sf::Keyboard::Unknown)
            uiManager().handleKeyRelease(event.key);
        if (event.key.code == sf::Keyboard::F12)
        {
            sf::Image tmp = window().capture();
            std::stringstream ssFile;
            ssFile << "screenshot_" << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << ".png";
            if (tmp.saveToFile(ssFile.str()))
                console().print(Console::Info, "Saved screenshot to '%s'", ssFile.str().c_str());
            else
                console().print(Console::Warning, "Failed to save screenshot");
        }
    }
    if (event.type == sf::Event::MouseWheelMoved)
        m_console.handleMouseWheel(event.mouseWheel.delta, event.mouseWheel.x, event.mouseWheel.y);
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (!console().isOpen())
            uiManager().handleMousePress(event.mouseButton);
    }
    if (event.type == sf::Event::MouseButtonReleased)
    {
        if (!console().isOpen())
            uiManager().handleMouseRelease(event.mouseButton);
    }
}

void Engine::beforeUpdate()
{
    // Do nothing by default
}

void Engine::onUpdate()
{
    inputManager().update();
    console().update(m_lastTime);
    camera().update();

    m_fpsString.setColor(sf::Color::White);
    m_fpsString.setPosition(config().settingInt("vid_width", 640) - 150,  8);

    if (config().settingBoolean("sys_showstats", false))
    {
        std::stringstream stats;
        stats << "Entity Count: "    << entityManager().entities().size() << std::endl;
        stats << "Texture Count: "   << resourceManager().textureCount() << std::endl;
        stats << "Sound Count: "     << resourceManager().soundCount() << std::endl;
        stats << "Live Sounds: "     << resourceManager().liveSoundCount() << std::endl;
        stats << "Music Count: "     << resourceManager().songCount() << std::endl;
        stats << "Font Count: "      << resourceManager().fontCount() << std::endl;
        if (m_currentState)
            stats << "Current State: "   << m_currentState->name() << std::endl;
        stats << "Camera Position: " << camera().position().x << " " << camera().position().y << std::endl;
        stats << "Camera Size: "     << camera().size().x << " " << camera().size().y << std::endl;
        stats << "World Size: "      << camera().world().x << " " << camera().world().y << std::endl;

        if (camera().lockedOn())
        {
            stats << "Camera Target: " << camera().lockedOn()->name() << std::endl;
            Player* player = dynamic_cast<Player*>(camera().lockedOn());

            if (player)
                stats << "Player Id: " << player->playerId() << std::endl;
        }
        m_statsString.setString(stats.str());
    }

    if (config().settingBoolean("r_clear", true))
        window().clear(config().settingColor("r_clearcolor", sf::Color::Black));

    if (m_currentState->type() == RunState::Game && !console().isOpen() && !m_paused)
    {
        entityManager().think(m_lastTime);
        entityManager().update(m_lastTime);
    }


    if (!console().isOpen())
    {
        if (m_currentState->isDone() && m_currentState->nextState() != NULL)
        {
            m_states.erase(m_currentState->name());
            RunState* oldState = m_currentState;
            m_currentState = oldState->nextState();

            delete oldState;
        }

        if (!m_currentState->isInitialized())
            m_currentState->initialize();

        uiManager().update(m_lastTime);
        m_currentState->update(m_lastTime);
    }

}

void Engine::afterUpdate()
{
    // Do nothing by default
}

void Engine::beforeDraw()
{
    // Do nothing by default
}

void Engine::onDrawEntities()
{
    // Entities need to be drawn to the camera's view
    if (m_currentState->type() == RunState::Game)
        entityManager().draw(window());
}

void Engine::onDraw()
{
    m_currentState->draw(window());
    uiManager().draw(window());
}

void Engine::onDrawConsole()
{

    if (config().settingBoolean("sys_showstats", false))
        window().draw(m_statsString);

    console().draw(window());

    if (config().settingBoolean("r_showfps", false))
        window().draw(m_fpsString);
}

void Engine::afterDraw()
{
    // Do nothing by default
}

void Engine::parseCommandLine()
{
    for (int i = 1; i < m_argc; i++)
    {
        console().print(Console::Message, "%s", m_argv[i]);
    }
}

void Engine::toggleFullscreen()
{
    // Default it to true so it will automatically be fullscreen
    setFullscreen(!config().settingBoolean("r_fullscreen", true));
}

bool Engine::consoleInitialized()
{
    return console().isInitialized();
}

void Engine::quit()
{
    window().close();
}

std::string Engine::version()
{
#ifndef INTERNAL
    return SAKURA_VERSION;
#else
    return SAKURA_VERSION + " Internal";
#endif
}

std::string Engine::gameVersion() const
{
    return "Unspecified";
}

std::string Engine::defaultTitle() const
{
    return "Sakura Engine " + Engine::version();
}

std::string prettySize(size_t size)
{
    float retSize = size;
    char buf[512];
    int i = 0;
    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (retSize > 1024) {
        retSize /= 1024.f;
        i++;
    }

    sprintf(buf, "%.*f %s", i, retSize, units[i]);
    return std::string(buf);
}

void Engine::printSysInfo()
{
    AsmJit::X86CpuInfo* cpuinfo = new AsmJit::X86CpuInfo;
    AsmJit::x86CpuDetect(cpuinfo);
    sf::ContextSettings context = m_window.getSettings();

    console().print(Console::Message, "***********************************************************");

    // CPU Info
    console().print(Console::Info, "CPU: %s %s with %i Cores and %i Max Threads.",
                    cpuinfo->getVendorString(),
                    cpuinfo->getBrandString(),
                    cpuinfo->getNumberOfProcessors(),
                    cpuinfo->getMaxLogicalProcessors());

    // RAM Info
    console().print(Console::Info, "RAM: %s", prettySize(getMemorySize()).c_str());

    // Graphics Info
    console().print(Console::Info, "GPU: %s %s OpenGL Version %s",
                    glGetString(GL_VENDOR),
                    glGetString(GL_RENDERER),
                    glGetString(GL_VERSION));

    // OpenGL Extensions
    std::string extension((const char*)glGetString(GL_EXTENSIONS));
    std::replace(extension.begin(), extension.end(), ' ', '\n');

    int dummy; // For some reason we need this...

    console().print(Console::Info, "GL_EXTENSIONS:\n%s",        extension.c_str());
    console().print(Console::Info, "Found %i GL Extensions", zelda::utility::countChar(extension, '\n',dummy));

    extension.clear();

    // Depth Buffer Bit..
    console().print(Console::Info, "GL_DEPTH_BUFFER_BIT  %i", context.depthBits);

    //  OpenAL
    if (alGetString(AL_VENDOR))
        console().print(Console::Info, "AL_VENDOR            %s", alGetString(AL_VENDOR));

    console().print(Console::Info, "ALC_DEVICE_SPECIFIER %s", alcGetString(NULL, ALC_DEVICE_SPECIFIER));  


    // ALC Extensions...

    char* str =  (char*)alcGetString(NULL, ALC_EXTENSIONS);

    if (str)
    {
        extension = std::string((const char*)str);
    }
    if (alGetString(AL_VERSION))
        console().print(Console::Info, "AL_VERSION           %s", alGetString(AL_VERSION));

    std::replace(extension.begin(), extension.end(), ' ', '\n');
    if (!extension.empty())
        console().print(Console::Info, "ALC_EXTENSIONS:\n%s", extension.c_str());

    // The End.
    extension.clear();

    console().print(Console::Message, "***********************************************************");
}

} // Core
} // Sakura
