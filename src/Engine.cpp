#include "Engine.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include <SFML/Config.hpp>

#include "Link.hpp"
#include "Octorok.hpp"
#include "HeartPickup.hpp"
#include <iostream>
#include "Widget.hpp"
#include "x86cpuinfo.h"
#include "FontResource.hpp"
#include "MusicResource.hpp"
#include "TextureResource.hpp"
#include "SoundResource.hpp"
#include "memorysize.h"
#include <sstream>
#include <AL/al.h>
#include <AL/alc.h>

Engine::Engine()
    : m_log("log.txt"),
      m_camera(sf::Vector2f(0, 0), sf::Vector2f(320, 240)),
      m_lastTime(sf::seconds(0)),
      m_fps(0),
      m_isClosing(0),
      m_drawWire(false),
      m_paused(false),
      m_state(StateSplash),
      m_currentLogo(LogoSFML),
      m_splashTime(sf::seconds(3.5f)),
      m_fade(0.0f),
      m_rotation(-90.f),
      m_fadeOut(false)
{
    log().print(Log::Info, "Sakura Engine ALPHA Initializing\n");
    log().print(Log::Info, "Built with SFML %i.%i\n", SFML_VERSION_MAJOR , SFML_VERSION_MINOR);
    log().print(Log::Info, "Build date %s %s\n", __DATE__, __TIME__);
}

Engine::~Engine()
{
    log().print(Log::Info, "Shutdown complete\n");
    std::cout << "Test" << std::endl;
}

void Engine::initialize(int argc, char* argv[], int width, int height, const std::string &title)
{
    ((void)argc);
    log().print(Log::Info, "Creating context...\n");
    m_window.create(sf::VideoMode(width, height), title);

    log().print(Log::Info, "Polling Hardware...\n");
    printSysInfo();
    log().print(Log::Info, "End Hardware poll\n");
    // Initailize the ResourceManager
    m_resourceManager.initialize(argv[0]);

    // Set state to StateSplash just to be sure and load the resources
    m_state = StateSplash;
    loadSplashResources();

    m_resourceManager.loadFont("fonts/debug", new FontResource("RetGanon.ttf", true));
    m_fpsString.setFont(*resourceManager().font("fonts/debug"));
    m_fpsString.setCharacterSize(16);
    m_fpsString.setColor(sf::Color::White);
    m_fpsBack.setOutlineColor(sf::Color(162, 108, 145));
    m_fpsBack.setOutlineThickness(1.f);
    m_fpsBack.setFillColor(sf::Color(183, 169, 199, 240));
    m_fpsBack.setSize(sf::Vector2f(m_fpsString.getLocalBounds().width + 16, m_fpsString.getLocalBounds().height + 16));
    ((sf::Texture&)m_fpsString.getFont()->getTexture(16)).setSmooth(false);
}

void Engine::doGameState()
{
    m_entityManager.think(m_lastTime);
    m_entityManager.update(m_lastTime);


    m_window.clear(sf::Color::Magenta);

    if (m_drawWire)
    {
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_entityManager.draw(m_window);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_TEXTURE_2D);
    }

    m_entityManager.draw(m_window);
}

int Engine::run()
{
    log().print(Log::Info, "Entering main loop...\n");
    while(m_window.isOpen())
    {
        sf::Time currentTime = m_clock.restart();
        m_fps = 1.f / currentTime.asSeconds();

        std::stringstream title;
        title << "FPS: " << m_fps;
        m_fpsString.setString(title.str());
        m_fpsBack.setSize(sf::Vector2f(72, m_fpsString.getLocalBounds().height + 8));

        //std::cout << '\xd' << "FPS: " << m_fps;

        if (m_isClosing)
            m_window.close();

        sf::Event event;
        while(m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_isClosing = true;
            if (event.type == sf::Event::GainedFocus)
                m_paused = false;
            if (event.type == sf::Event::LostFocus)
                m_paused = true;
            if (event.type == sf::Event::Resized)
                m_camera.resize(event.size.width, event.size.height);
        }

        m_lastTime = currentTime;
        m_camera.update();
        m_inputManager.update();
        m_fpsString.setColor(sf::Color::White);
        m_fpsString.setPosition(m_camera.position().x + 8.f, (m_camera.position().y  + m_camera.size().y) - (m_fpsString.getLocalBounds().height*2));
        m_fpsBack.setPosition(m_camera.position().x + 6.f, (m_camera.position().y  + m_camera.size().y) - ((m_fpsString.getLocalBounds().height*2) + 2));
        m_window.setView(m_camera.view());

        switch(m_state)
        {
            case StateSplash:
                doSplashState(m_lastTime);
                break;
            case StateMenu:
                doMenuState();
                break;
            case StateGame:
                doGameState();
                break;
        }

        m_window.draw(m_fpsBack);
        m_window.draw(m_fpsString);
        m_window.display();
    }
    log().print(Log::Info, "Quit main loop performing preshutdown maintenance...\n");
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

Log& Engine::log()
{
    return m_log;
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
    resourceManager().removeTexture("splash/SFML");
    resourceManager().removeTexture("splash/NintendoLogo");
    resourceManager().removeTexture("splash/Wiiking");
}

void Engine::shutdown()
{
    log().print(Log::Info, "Killing Entity Manager...\n");
    m_entityManager.shutdown();
    log().print(Log::Info, "Killing Resource Manager...\n");
    m_resourceManager.shutdown();
    log().print(Log::Info, "Maintenance complete...\n");
}

void Engine::doSplashState(sf::Time dt)
{
    if (m_paused) return;

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
            m_logoSprite.setTexture(*resourceManager().texture("splash/SFML"));
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
    m_window.clear();

    m_window.draw(m_logoSprite);
}

void Engine::doMenuState()
{
    log().print(Log::Warning, "In doMenuState() IMPLEMENT ME\n");
    m_state = StateGame;
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

    log().print(Log::Message, "***********************************************************\n");
    log().print(Log::Info, "CPU\t\t\t%s\n",         cpuinfo->getBrandString());
    log().print(Log::Info, "CPU Vendor\t\t%s\n",    cpuinfo->getVendorString());
    log().print(Log::Info, "CPU Cores\t\t%i\n",     cpuinfo->getNumberOfProcessors());
    log().print(Log::Info, "CPU Max Threads\t%i\n", cpuinfo->getMaxLogicalProcessors());
    log().print(Log::Info, "System RAM\t\t%s\n",    prettySize(getMemorySize()).c_str());
    log().print(Log::Info, "GL_VENDOR\t\t%s\n",     glGetString(GL_VENDOR));
    log().print(Log::Info, "GL_RENDERER\t\t%s\n",   glGetString(GL_RENDERER));
    log().print(Log::Info, "GL_VERSION\t\t%s\n",    glGetString(GL_VERSION));

    std::string extension((const char*)glGetString(GL_EXTENSIONS));
    std::replace(extension.begin(), extension.end(), ' ', '\n');
    extension.erase(extension.end() - 1, extension.end());

    log().print(Log::Info, "GL_EXTENSIONS:\n%s\n",        extension.c_str());
    log().print(Log::Info, "GL_DEPTH_BUFFER_BIT\t%i\n", context.depthBits);
    if (alGetString(AL_VENDOR))
        log().print(Log::Info, "AL_VENDOR\t\t%s\n", alGetString(AL_VENDOR));

    log().print(Log::Info, "ALC_DEVICE_SPECIFIER\t%s\n", alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    char* str;
    extension.clear();
    str =  (char*)alcGetString(NULL, ALC_EXTENSIONS);
    if (str)
    {
        extension = std::string((const char*)str);
        std::replace(extension.begin(), extension.end(), ' ', '\n');
    }
    if (alGetString(AL_VERSION))
        log().print(Log::Info, "AL_VERSION\t%s\n", alGetString(AL_VERSION));

    if (!extension.empty())
        log().print(Log::Info, "ALC_EXTENSIONS:\n%s\n", extension.c_str());

    log().print(Log::Message, "***********************************************************\n");
}
