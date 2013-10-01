#include "Engine.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include <SFML/Config.hpp>

#include <Entity.hpp>
#include <iostream>
#include <iomanip>
#include "x86cpuinfo.h"
#include "FontResource.hpp"
#include "MusicResource.hpp"
#include "TextureResource.hpp"
#include "SoundResource.hpp"
#include "memorysize.h"

#include "MapFileReader.hpp"
#include "Map.hpp"
#include <sstream>
#include <AL/al.h>
#include <AL/alc.h>
#include <mutex>


const std::string Engine::SAKURA_VERSION = "v0.1a";

void InputThread()
{
    while(Engine::instance().window().isOpen())
    {
        Engine::instance().inputManager().update();
        sf::sleep(sf::milliseconds(20));
    }
}

Engine::Engine()
    : m_console("log.txt"),
      m_camera(sf::Vector2f(100, 100), sf::Vector2f(320, 240)),
      m_lastTime(sf::seconds(0)),
      m_frameLimit(0),
      m_fps(0),
      m_paused(false),
      m_inputThreadInitialized(false),
      m_clearColor(sf::Color::Black),
      m_currentMap(NULL)
{
    console().print(Console::Info, "Sakura Engine " + version() + ": Initializing");
    console().print(Console::Info, "Built with SFML %i.%i", SFML_VERSION_MAJOR , SFML_VERSION_MINOR);
    console().print(Console::Info, "Build date %s %s", __DATE__, __TIME__);
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

    m_title = config().settingLiteral("sys_title", "Sakura Engine " + Engine::version());
    m_size = sf::Vector2u(config().settingInt("vid_width", 640), config().settingInt("vid_height", 480));
    console().print(Console::Info, "Creating context...");
    setFullscreen(config().settingBoolean("r_fullscreen", false));



    m_frameLimit = config().settingInt("sys_framelimit", 60);
    window().setFramerateLimit(m_frameLimit);

    window().setVerticalSyncEnabled(config().settingBoolean("sys_vsync", true));
    // QUICK GRAB THE VIEW!!!
    m_defaultView = window().getDefaultView();

    console().print(Console::Info, "Polling Hardware...");
    printSysInfo();
    console().print(Console::Info, "End Hardware poll");
    // Initailize the ResourceManager
    resourceManager().initialize(argv[0]);
    resourceManager().loadFont("fonts/debug", new FontResource("courbd.ttf", true));
    m_clearColor = config().settingColor("r_clearcolor", sf::Color::Black);


    // only initialize the inputthread if it's not already intialized
    // Don't want to have stray threads littering the place
    if (!m_inputThreadInitialized)
    {
        m_inputThread = std::thread(&InputThread);
        m_inputThread.detach();
        m_inputThreadInitialized = true;
    }

    // Initialize the console
    m_console.initialize();

    // Set state to StateSplash just to be sure and load the resources
    //m_state = StateSplash;
    //loadSplashResources();


    if (resourceManager().fontExists("fonts/debug"))
    {
        m_fpsString.setFont(*resourceManager().font("fonts/debug"));
        m_fpsString.setCharacterSize(20);
        m_fpsString.setColor(sf::Color::White);
        ((sf::Texture&)m_fpsString.getFont()->getTexture(20)).setSmooth(false);
        m_statsString.setFont(*resourceManager().font("fonts/debug"));
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
    console().print(Console::Info, "Entering main loop...");
    while(window().isOpen())
    {
        if (config().settingInt("sys_framelimit", 60) != m_frameLimit)
        {
            m_frameLimit = config().settingInt("sys_framelimit", 60);
            window().setFramerateLimit(m_frameLimit);
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
                m_console.handleInput(event.key.code, event.key.alt, event.key.control, event.key.shift, event.key.system);
            if (event.type == sf::Event::MouseWheelMoved)
                m_console.handleMouseWheel(event.mouseWheel.delta, event.mouseWheel.x, event.mouseWheel.y);
        }

        camera().update();

        console().update(m_lastTime);
        m_fpsString.setColor(sf::Color::White);
        m_fpsString.setPosition(config().settingInt("vid_width", 640) - 150,  8);

        if (config().settingBoolean("sys_showstats", false))
        {
            std::stringstream stats;
            stats << "Entity Count: " << entityManager().entities().size() << std::endl;
            stats << "Texture Count: " << resourceManager().textureCount() << std::endl;
            stats << "Sound Count: " << resourceManager().soundCount() << std::endl;
            stats << "Live Sounds: " << resourceManager().liveSoundCount() << std::endl;
            stats << "Music Count: " << resourceManager().musicCount() << std::endl;
            stats << "Font Count: " << resourceManager().fontCount() << std::endl;
            stats << "Camera Position: " << camera().position().x << " " << camera().position().y << std::endl;
            stats << "Camera Size: " << camera().size().x << " " << camera().size().y << std::endl;
            stats << "World Size: " << camera().world().x << " " << camera().world().y << std::endl;
            if (camera().lockedOn())
            {
                stats << "Camera Target: " << camera().lockedOn()->name() << std::endl;
            }

            m_statsString.setString(stats.str());
        }

        if (config().settingBoolean("r_clear", true))
            window().clear(m_clearColor);

        if (!console().isOpen())
        {
            entityManager().think(m_lastTime);
            entityManager().update(m_lastTime);
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

            if (config().settingBoolean("sys_showstats", false))
                window().draw(m_statsString);

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

Engine& Engine::instance()
{
    static Engine engine;
    return engine;
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


    console().print(Console::Info, "Killing Entity Manager...");
    entityManager().shutdown();
    console().print(Console::Info, "Killing Resource Manager...");
    entityManager().shutdown();
    console().print(Console::Info, "Maintenance complete...");
    resourceManager().shutdown();
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
