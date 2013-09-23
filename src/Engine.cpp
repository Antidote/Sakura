#include "Engine.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include <SFML/Config.hpp>

#include "Link.hpp"
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
      m_splashTime(sf::seconds(3.5f)),
      m_frameLimit(0),
      m_fps(0),
      m_paused(false),
      m_state(StateGame),
      m_currentLogo(LogoSFML),
      m_fade(0.0f),
      m_rotation(-90.f),
      m_fadeOut(false),
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

    zelda::io::MapFileReader reader("data/maps/EasternPalace.zmap");
    m_currentMap = reader.read();
    camera().setWorld(m_currentMap->width(), m_currentMap->height());
    m_colShape.setSize(sf::Vector2f(m_currentMap->tileWidth(), m_currentMap->tileHeight()));
    Link* link = new Link;
    link->setPosition(100, 32);
    camera().setLockedOn(link);
    entityManager().addEntity(link);
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
            stats << "Current State: " << stateString() << std::endl;
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

        if (m_state == StateGame && !console().isOpen())
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

            switch(m_state)
            {
                case StateSplash:
                    doSplashState(m_lastTime);
                    break;
                case StateMenu:
                    doMenuState();
                    break;
                case StateGame:
                    doGameState(config().settingBoolean("r_drawwire", false), i);
                    break;
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

void Engine::loadSplashResources()
{
    if (!resourceManager().soundExists("sounds/items/rupeeCollect"))
        resourceManager().loadSound("sounds/items/rupeeCollect", new SoundResource("sounds/LTTP_Rupee1.wav"));

    if (!resourceManager().textureExists("splash/SFML"))
        resourceManager().loadTexture("splash/SFML", new TextureResource("splash/SFML.png", true));

    if (!resourceManager().textureExists("splash/NintendoLogo"))
        resourceManager().loadTexture("splash/NintendoLogo", new TextureResource("splash/NintendoLogo.png", true));

    if (!resourceManager().textureExists("splash/Wiiking"))
        resourceManager().loadTexture("splash/Wiiking", new TextureResource("splash/Wiiking.png", true));
}

void Engine::destroySplashResources()
{
    console().print(Console::Info, "Clearing logo resources...");
    resourceManager().removeMusic  ("sounds/items/rupeeCollect");
    resourceManager().removeTexture("splash/SFML");
    resourceManager().removeTexture("splash/NintendoLogo");
    resourceManager().removeTexture("splash/Wiiking");
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
    // Reset everything
    m_state = Engine::StateGame;
    m_logoSprite = sf::Sprite();
    m_fade = 0.0f;
    m_fadeOut = false;
    m_currentSplashTime = sf::seconds(0.0f);
    m_currentLogo = 0;
}

void Engine::doSplashState(sf::Time dt)
{
    if (!m_paused && !m_console.isOpen())
    {
        //setClearColor(sf::Color::Transparent);
        m_currentSplashTime += dt;

        if (m_currentSplashTime > m_splashTime)
        {
            m_currentSplashTime = sf::seconds(0.f);
            if (m_fade <= 0)
            {
                m_currentLogo++;
                if (m_currentLogo == LogoWiiking)
                    m_rotation = -180.f;
            }


            m_fadeOut ^= 1;
        }

        if (m_currentLogo >= LogoCount)
        {
            m_state = StateMenu;
            destroySplashResources();
            return;
        }

        if (!m_fadeOut && m_fade < 255)
            m_fade += 90.f*dt.asSeconds();
        else if (m_fadeOut && m_fade > 0)
            m_fade -= 90.f*dt.asSeconds();

        // Fixes flashing
        // Since m_fade is a float it can go over 255 and overflow the Uint8
        // resonsible for Alpha, Ask Laurent to add float values for color?
        if (m_fade < 0)
            m_fade = 0.f;
        if (m_fade > 255.f)
            m_fade = 255.f;

        if (m_currentLogo == LogoWiiking && m_rotation < 0)
            m_rotation += 48.f*dt.asSeconds();

        if (m_rotation > 0)
        {
            resourceManager().playSound("sounds/items/rupeeCollect");
            m_rotation = 0;
        }

        m_logoSprite.setPosition((camera().position().x + (camera().size().x / 2)),
                                 (camera().position().y + (camera().size().y / 2)));

        if ((Logo)m_currentLogo == LogoSFML)
        {
            if (resourceManager().textureExists("splash/SFML") &&
                    m_logoSprite.getTexture() != resourceManager().texture("splash/SFML"))
            {
                m_logoSprite.setTexture(*resourceManager().texture("splash/SFML"), true);
                m_logoSprite.setOrigin(m_logoSprite.getLocalBounds().width/2, m_logoSprite.getLocalBounds().height/2);

            }
        }

        if ((Logo)m_currentLogo == LogoNintendo)
        {
            if (resourceManager().textureExists("splash/NintendoLogo") &&
                    m_logoSprite.getTexture() != resourceManager().texture("splash/NintendoLogo"))
            {
                m_logoSprite.setTexture(*resourceManager().texture("splash/NintendoLogo"), true);
                m_logoSprite.setOrigin(m_logoSprite.getLocalBounds().width/2, m_logoSprite.getLocalBounds().height/2);

            }
        }

        if ((Logo)m_currentLogo == LogoWiiking)
        {
            if (resourceManager().textureExists("splash/Wiiking") &&
                    m_logoSprite.getTexture() != resourceManager().texture("splash/Wiiking"))
            {
                m_logoSprite.setTexture(*resourceManager().texture("splash/Wiiking"), true);
                m_logoSprite.setScale(.5f, .5f);
                m_logoSprite.setOrigin(m_logoSprite.getLocalBounds().width/2, m_logoSprite.getLocalBounds().height/2);
                ((sf::Texture*)m_logoSprite.getTexture())->setSmooth(true);
            }

            m_logoSprite.setRotation(m_rotation);
        }

        m_logoSprite.setColor(sf::Color(255, 255, 255, m_fade));
    }

    window().draw(m_logoSprite);
}

void Engine::doMenuState()
{
}

void Engine::doGameState(bool wireframe, int pass)
{
    setClearColor(sf::Color(m_currentMap->backgroundColor().R, m_currentMap->backgroundColor().G, m_currentMap->backgroundColor().B, m_currentMap->backgroundColor().A));

    if ((wireframe && pass == 1) || !wireframe)
    {
        if (m_currentMap)
        {
            for (Uint32 y = 0; y < m_currentMap->height()/m_currentMap->tileHeight(); y++)
            {
                if ((y * m_currentMap->tileHeight()) < camera().position().y - m_currentMap->tileHeight() ||
                        (y * m_currentMap->tileHeight()) > camera().position().y + camera().size().y)
                    continue;

                for (Uint32 x = 0; x < m_currentMap->width()/m_currentMap->tileWidth(); x++)
                {
                    if ((x * m_currentMap->tileWidth()) < camera().position().x - m_currentMap->tileWidth() ||
                            (x * m_currentMap->tileWidth()) > camera().position().x + camera().size().x)
                        continue;
                    Cell* cell = m_currentMap->collision(x, y);
                    if (cell)
                    {
                        if (cell->CollisionType == ColTypeNone)
                            continue;

                        if ((cell->CollisionType & ColTypeAngle45) == ColTypeAngle45)
                            m_colShape.setFillColor(sf::Color::Yellow);
                        else if ((cell->CollisionType & ColTypeJump) == ColTypeJump)
                            m_colShape.setFillColor(sf::Color::Green);
                        else if ((cell->CollisionType & ColTypeWaterShallow) == ColTypeWaterShallow)
                            m_colShape.setFillColor(sf::Color(200, 24, 255));
                        else if ((cell->CollisionType & ColTypeWaterDeep) == ColTypeWaterDeep)
                            m_colShape.setFillColor(sf::Color::Blue);
                        else if ((cell->CollisionType & ColTypeDamage) == ColTypeDamage)
                            m_colShape.setFillColor(sf::Color::Red);
                        else if ((cell->CollisionType & ColTypeStair) == ColTypeStair)
                            m_colShape.setFillColor(sf::Color(100, 68, 255));
                        else
                            m_colShape.setFillColor(sf::Color::Black);

                        m_colShape.setPosition(x * m_currentMap->tileWidth(), y * m_currentMap->tileHeight());


                        window().draw(m_colShape);
                    }
                }
            }
        }
    }
    entityManager().draw(window());
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

std::string Engine::stateString()
{
    switch(m_state)
    {
        case StateSplash:
            return "Splash";

        case StateMenu:
            return "Menu";

        case StateGame:
            return "Game";
    }

    return "Unknown";
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
