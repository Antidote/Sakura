#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/RunState.hpp"
#include "Sakura/Core/Player.hpp"
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
const std::string Engine::SAKURA_VERSION = "0.1a";

void InputThread()
{
    while(sEngineRef().window().isOpen())
    {
        sEngineRef().inputManager().update();
        sf::sleep(sf::milliseconds(20));
    }
}

Engine::Engine()
    : m_console("log.txt"),
      m_camera(sf::Vector2f(100, 100), sf::Vector2f(320, 240)),
      m_lastTime(sf::seconds(0)),
      m_frameLimit(0),
      m_vsync(false),
      m_fps(0),
      m_paused(false),
      m_inputThreadInitialized(false),
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
}

Engine::~Engine()
{
    console().print(Console::Info, "Shutdown complete");
    m_inputThreadInitialized = false;
}

void Engine::initialize(int argc, char* argv[])
{
    m_argc = argc;
    m_argv = argv;

    config().initialize("config.cfg");

    m_title = config().settingLiteral("sys_title", defaultTitle());
    m_size = sf::Vector2u(config().settingInt("vid_width", 640), config().settingInt("vid_height", 480));
    console().print(Console::Info, "Creating context...");
    setFullscreen(config().settingBoolean("r_fullscreen", false));

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
    resourceManager().initialize(argv[0]);

    resourceManager().loadFont("fonts/debug.ttf", true);
    m_clearColor = config().settingColor("r_clearcolor", sf::Color::Black);

/*
    // only initialize the inputthread if it's not already intialized
    // Don't want to have stray threads littering the place
    if (!m_inputThreadInitialized)
    {
        m_inputThread = std::thread(&InputThread);
        m_inputThread.detach();
        m_inputThreadInitialized = true;
    }
*/
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
    initialize(m_argc, m_argv);

    if (wasFullscreen)
        setFullscreen(true);
}

int Engine::run()
{
    if (!m_currentState && m_states.size() <= 0)
    {
        console().print(Console::Fatal, "No states specified!!!");
    }

    console().print(Console::Info, "Entering main loop...");
    while(window().isOpen())
    {
        if (config().settingInt("sys_framelimit", 60) != m_frameLimit)
        {
            m_frameLimit = config().settingInt("sys_framelimit", 60);
            window().setFramerateLimit(m_frameLimit);
        }

        if (config().settingBoolean("sys_vsync", true) != m_vsync)
        {
            m_vsync = config().settingBoolean("sys_vsync", true);
            window().setVerticalSyncEnabled(m_vsync);
        }

        m_lastTime = m_clock.restart();
        m_fps = 1.f / m_lastTime.asSeconds();

        std::stringstream fpsStr;
        fpsStr << std::setprecision(4) << "FPS: " << m_fps;
        m_fpsString.setString(fpsStr.str());

        sf::Event event;
        while(m_window.pollEvent(event))
        {
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

        inputManager().update();
        camera().update();
        console().update(m_lastTime);

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

        if (m_currentState->type() == RunState::Game && !console().isOpen())
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
                if (!m_currentState->isInitialized())
                    m_currentState->initialize();

                delete oldState;
            }
            uiManager().update(m_lastTime);
            m_currentState->update(m_lastTime);
        }

        for (int i = 0; i < (config().settingBoolean("r_drawwire", false) ? 2 : 1); i++)
        {
            window().setView(m_camera.view());
            if (config().settingBoolean("r_drawwire", false) && i == 1)
            {
                glDisable(GL_TEXTURE_2D);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }

            // Reset the view to the default
            // Draw the console over everything
            window().setView(m_defaultView);

            m_currentState->draw(window());
            if (m_currentState->type() == RunState::Game)
                entityManager().draw(window());

            if (config().settingBoolean("sys_showstats", false))
                window().draw(m_statsString);

            uiManager().draw(window());

            console().draw(window());

            if (config().settingBoolean("r_showfps", false))
                window().draw(m_fpsString);

            if (config().settingBoolean("r_drawwire", false) && i == 1)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEnable(GL_TEXTURE_2D);
            }
        }

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

    console().print(Console::Info, "Killing Entity Manager...");
    entityManager().shutdown();
    console().print(Console::Info, "Killing Resource Manager...");
    resourceManager().shutdown();
    console().print(Console::Info, "Maintenance complete...");
    console().shutdown();
    config().shutdown();
}

void Engine::setWireframe(bool mode)
{
    config().setSettingBoolean("r_drawwire", mode);
}

void Engine::setFullscreen(bool isFullscreen)
{
    // Don't toggle if it's the same
    if (isFullscreen == config().settingBoolean("r_fullscreen", false) && window().isOpen())
        return;

    window().close();
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
    if (!m_currentState->isInitialized())
        m_currentState->initialize();
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
    {
        m_currentState = newState;
        // If we don't initialize the initial gamestate the engine will choke.
        // TODO: Find out why
        m_currentState->initialize();
    }
}

Map* Engine::currentMap() const
{
    return m_currentMap;
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
    console().print(Console::Info, "CPU                  %s", cpuinfo->getBrandString());
    console().print(Console::Info, "CPU Vendor           %s", cpuinfo->getVendorString());
    console().print(Console::Info, "CPU Cores            %i", cpuinfo->getNumberOfProcessors());
    console().print(Console::Info, "CPU Max Threads      %i", cpuinfo->getMaxLogicalProcessors());
    console().print(Console::Info, "System RAM           %s", prettySize(getMemorySize()).c_str());
    console().print(Console::Info, "GL_VENDOR            %s", glGetString(GL_VENDOR));
    console().print(Console::Info, "GL_RENDERER          %s", glGetString(GL_RENDERER));
    console().print(Console::Info, "GL_VERSION           %s", glGetString(GL_VERSION));

    std::string extension((const char*)glGetString(GL_EXTENSIONS));
    std::replace(extension.begin(), extension.end(), ' ', '\n');

    console().print(Console::Info, "GL_EXTENSIONS:\n%s",        extension.c_str());
    int lastOccur;
    console().print(Console::Info, "Found %i GL Extensions", zelda::utility::countChar(extension, '\n', lastOccur));
    console().print(Console::Info, "GL_DEPTH_BUFFER_BIT  %i", context.depthBits);
    if (alGetString(AL_VENDOR))
        console().print(Console::Info, "AL_VENDOR            %s", alGetString(AL_VENDOR));

    console().print(Console::Info, "ALC_DEVICE_SPECIFIER %s", alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    char* str;
    extension.clear();
    str =  (char*)alcGetString(NULL, ALC_EXTENSIONS);
    if (str)
    {
        extension = std::string((const char*)str);
    }
    if (alGetString(AL_VERSION))
        console().print(Console::Info, "AL_VERSION           %s", alGetString(AL_VERSION));

    std::replace(extension.begin(), extension.end(), ' ', '\n');
    if (!extension.empty())
        console().print(Console::Info, "ALC_EXTENSIONS:\n%s", extension.c_str());

    console().print(Console::Message, "***********************************************************");
}

} // Core
} // Sakura
