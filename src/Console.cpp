#include "Console.hpp"
#include "Engine.hpp"
#include "TextureResource.hpp"
#include <iostream>

#include <stdarg.h>

#ifdef SFML_SYSTEM_LINUX
#include <X11/keysym.h>
#endif

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <utility.hpp>


Console::Console(const std::string &logfile)
    : m_state(Closed),
      m_showCursor(false),
      m_totalWaitTime(sf::seconds(0.5)),
      m_cursorPosition(0),
      m_blinkTimeout(sf::seconds(.5f)),
      m_isInitialized(false),
      m_overwrite(false),
      m_startString(0),
      m_currentCommand(0),
      m_log(logfile),
      m_maxLines(15),
      m_conHeight(240),
      m_conY(0.0f),
      m_defaultConColor(sf::Color(100, 180, 32, 100))
{
}

Console::~Console()
{
    shutdown();
}

void Console::initialize()
{
    // register default commands
    m_commands["quit"] = new QuitCommand();
    m_commands["r_fullscreen"] = new FullscreenCommand();
    m_commands["r_drawwire"] = new WireframeCommand();
    m_commands["con_clear"] = new ClearCommand();
    m_commands["play"] = new PlayCommand();
    m_commands["bind"] = new BindCommand();
    m_commands["unbind"] = new UnbindCommand();
    m_commands["restart"] = new RestartCommand();

    m_borderShape.setPosition(1, 1);
    m_borderShape.setFillColor(sf::Color::Transparent);
    m_borderShape.setOutlineColor(sf::Color::Red);
    m_borderShape.setOutlineThickness(1.f);

    m_conHeight = Engine::instance().config().settingInt("con_height", 240);

    if (Engine::instance().resourceManager().fontExists("fonts/debug"))
    {
        m_commandText.setFont(*Engine::instance().resourceManager().font("fonts/debug"));
        m_commandText.setCharacterSize(15);
        ((sf::Texture&)m_commandText.getFont()->getTexture(m_commandText.getCharacterSize())).setSmooth(false);
        m_drawText.setFont(*Engine::instance().resourceManager().font("fonts/debug"));
        m_drawText.setCharacterSize(15);
        ((sf::Texture&)m_drawText.getFont()->getTexture(m_drawText.getCharacterSize())).setSmooth(false);
        recalcMaxLines();
    }
    else
    {
        print(Fatal, "Unable to access font resource!");
    }

    if (!Engine::instance().resourceManager().textureExists("console/bg1"))
        Engine::instance().resourceManager().loadTexture("console/bg1", new TextureResource("console/bg1.png", true));
    if (!Engine::instance().resourceManager().textureExists("console/bg2"))
        Engine::instance().resourceManager().loadTexture("console/bg2", new TextureResource("console/bg2.png", true));

    // double check before setting JUST in case
    if (Engine::instance().resourceManager().textureExists("console/bg1"))
    {
        m_consoleBg1.setTexture(*Engine::instance().resourceManager().texture("console/bg1"));
        ((sf::Texture*)m_consoleBg1.getTexture())->setRepeated(true);
        ((sf::Texture*)m_consoleBg1.getTexture())->setSmooth(false);
    }

    if (Engine::instance().resourceManager().textureExists("console/bg2"))
    {
        m_consoleBg2.setTexture(*Engine::instance().resourceManager().texture("console/bg2"));
        ((sf::Texture*)m_consoleBg2.getTexture())->setRepeated(true);
        ((sf::Texture*)m_consoleBg2.getTexture())->setSmooth(false);
    }
    else
    {
    }
    m_isInitialized = true;
}

void Console::shutdown()
{
    m_isInitialized = false;
    m_state = Closed;
    m_showCursor = true;
    m_cursorTime = sf::seconds(0.0f);
    m_showError = true;
    m_hadFatalError = false;
    m_conY = 0.0f;
    // Time to unregister all commands
    for (std::pair<std::string, ConsoleCommandBase*> cmd : m_commands)
    {
        delete cmd.second;
        cmd.second = NULL;
    }

    m_commands.clear();
}

bool Console::isOpen() const
{
    return m_state != Closed;
}

void Console::handleText(const sf::Uint32 unicode)
{

    // SFML Currently doesn't properly support the '`' key
    // (XK_grave and XK_dead_grave) on some systems, this is a work around.
    // Why this key in particular is such a pain in the neck is beyond me.
#ifdef SFML_SYSTEM_LINUX
    if (unicode == XK_grave || unicode == XK_dead_grave)
    {
        if (Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::LControl) ||
            Engine::instance().inputManager().keyboard().isKeyDown(sf::Keyboard::RControl))
            m_conHeight = Engine::instance().config().settingInt("vid_height", 480);
        else
            m_conHeight = Engine::instance().config().settingInt("con_height", 320);

        toggleConsole();
        return;
    }
#endif

    if (m_state == Opened)
    {
        if (unicode > 0x1f && unicode != 0x7f)
        {
            if(!m_overwrite)
                m_commandString.insert(m_cursorPosition, unicode);

            if (m_overwrite && (m_commandString.getSize() > 0 && m_cursorPosition < m_commandString.getSize()))
                m_commandString[m_cursorPosition] = unicode;
            else if (m_overwrite)
                m_commandString += unicode;

            m_cursorPosition++;
            if (m_drawText.getFont())
                m_cursorX += (m_drawText.getFont()->getGlyph(m_drawText.getString()[m_cursorPosition], m_drawText.getCharacterSize(), false).advance);
        }
    }
}

void Console::toggleConsole()
{
    if (m_state == Closed || m_state == Closing)
    {
        m_state = Opening;
    }
    else
    {
        m_state = Closing;
    }

    m_commandString = "";
    m_cursorPosition = 0;

    if (m_hadFatalError)
        Engine::instance().window().close();
}

void Console::handleInput(sf::Keyboard::Key code, bool alt, bool control, bool shift, bool system)
{
    // shutup compiler, i might need these later
    UNUSED(shift);
    UNUSED(system);
    // unknown key, don't? Well what do we want with it?
    if (code == sf::Keyboard::Unknown)
        return;

    // Anything but linux should be safe
    // from bug mentioned in handleText
    if (code == sf::Keyboard::Tilde)
        toggleConsole();

    // Handle toggling of fullscreen (is this the best place for this?)
    if (code == sf::Keyboard::Return && alt)
    {
        Engine::instance().toggleFullscreen();
        return;
    }
    if (code != sf::Keyboard::Unknown && code == Engine::instance().config().keyForAction("quit", sf::Keyboard::Unknown) && (m_state != Opened && m_state != Opening))
    {
        Engine::instance().window().close();
        return;
    }

    // if the console isn't open there isn't any need to process commands
    if (m_state != Opened)
        return;

    switch(code)
    {

        break;
        case sf::Keyboard::BackSpace:
            if (m_commandString.getSize() > 0)
            {
                m_cursorX -= (m_drawText.getFont()->getGlyph(m_drawText.getString()[m_cursorPosition - 1], m_drawText.getCharacterSize(), false).advance);

                m_commandString.erase(--m_cursorPosition);
            }
            break;
        case sf::Keyboard::Delete:
        {
            if (m_commandString.getSize() > 0)
            {
                // Don't try to delete if the cursor is at the end of the line
                if (m_cursorPosition >= m_commandString.getSize())
                    break;
                m_commandString.erase(m_cursorPosition);
            }
        }
            break;

        case sf::Keyboard::PageUp:
        {
            if (control && ((m_history.size()) - (m_startString + m_maxLines)) > m_maxLines)
            {
                m_startString += m_maxLines;
                break;
            }
            if (((m_history.size())  - m_startString) > m_maxLines)
                m_startString++;
        }
            break;
        case sf::Keyboard::PageDown:
        {
            if (m_startString > 0)
            {
                if (m_startString >= m_maxLines && control)
                {
                    m_startString -= m_maxLines;
                    break;
                }
                m_startString--;
            }
        }
            break;
        case sf::Keyboard::Return:
        {
            parseCommand();
        }
            break;
        case sf::Keyboard::Left:
        {
            if (m_cursorPosition <= 0)
                break;
            m_cursorPosition--;
            m_cursorX -= (m_drawText.getFont()->getGlyph(m_drawText.getString()[m_cursorPosition - 1], m_drawText.getCharacterSize(), false).advance);
            m_showCursor = true;
            m_cursorTime = sf::seconds(0.f);
        }
            break;
        case sf::Keyboard::Right:
        {
            if (m_cursorPosition >= m_commandString.getSize())
                break;
            m_cursorPosition++;
            m_cursorX += (m_drawText.getFont()->getGlyph(m_drawText.getString()[m_cursorPosition - 1], m_drawText.getCharacterSize(), false).advance);
            m_showCursor = true;
            m_cursorTime = sf::seconds(0.f);
        }
            break;

        case sf::Keyboard::Up:
        {
            if (m_commandHistory.size() == 0)
                break;
            m_currentCommand++;
            if (m_currentCommand < m_commandHistory.size())
            {
                m_commandString = m_commandHistory[m_currentCommand];
            }
            else
            {
                m_currentCommand = m_commandHistory.size() - 1;
                m_commandString = m_commandHistory[m_commandHistory.size() - 1];
            }
            m_drawText.setString(m_commandString);
            m_cursorX = m_drawText.getLocalBounds().width;
            m_cursorPosition = m_commandString.getSize();
        }
            break;

        case sf::Keyboard::Down:
        {
            if (m_commandHistory.size() == 0)
                break;
            m_currentCommand--;
            if (m_currentCommand >= 0)
            {
                m_commandString = m_commandHistory[m_currentCommand];
            }
            else
            {
                m_currentCommand = 0;
                m_commandString = m_commandHistory[0];
            }
            m_drawText.setString(m_commandString);
            m_cursorX = m_drawText.getLocalBounds().width;
            m_cursorPosition = m_commandString.getSize();
        }
            break;

        case sf::Keyboard::Insert: m_overwrite ^= 1; break;
        case sf::Keyboard::Home:
            m_cursorPosition = 0;
            m_drawText.setString(m_commandString);
            m_cursorX = 0;
            m_showCursor = true;
            m_cursorTime = sf::seconds(0.f);
            break;
        case sf::Keyboard::End:
            m_cursorPosition = m_commandString.getSize();
            m_drawText.setString(m_commandString);
            m_cursorX = m_drawText.getLocalBounds().width;
            m_showCursor = true;
            m_cursorTime = sf::seconds(0.f);
            break;

        default:
            break;
    }

    if (m_cursorPosition > m_commandString.getSize())
        m_cursorPosition = m_commandString.getSize();
    if (m_cursorPosition < 0)
        m_cursorPosition = 0;
}

void Console::handleMouseWheel(int delta, int x, int y)
{
    // Mouse scrolling
    // because why not.

    // If the mouse is currently outside the consoles bounds
    // we don't need to do anything
    if (!m_borderShape.getLocalBounds().contains(x, y))
        return;

    if (delta < 0 && m_startString == 0)
        return;
    if (delta > 0 && (m_history.size() - (m_startString + delta*4)) < m_maxLines)
        return;

    m_startString += delta*4;
}

void Console::update(const sf::Time& dt)
{
    m_maxLines = std::ceil((m_conY / m_drawText.getCharacterSize()) - 4);
    if (m_state == Opening && m_conY < m_conHeight)
    {
        if (m_conY < m_conHeight)
            m_conY += dt.asSeconds()*Engine::instance().config().settingFloat("con_speed", 200.f);
    }
    else if (m_state == Opening && m_conY > m_conHeight)
    {
        m_state = Opened;
        m_conY = m_conHeight;
    }

    if (m_state == Closed)
        return;

    if (m_state == Closing && m_conY > m_drawText.getCharacterSize())
    {
        m_conY -= dt.asSeconds()*Engine::instance().config().settingFloat("con_speed", 200.f);
    }
    else if (m_conY < m_drawText.getCharacterSize() && m_state == Closing)
    {
        m_state = Closed;
        m_conY = 0;
        m_startString = 0;
    }

    m_currentTime = dt;

    m_cursorTime += dt;
    if (m_cursorTime >= m_blinkTimeout)
    {
        m_showCursor ^= 1;
        m_showError ^= 1;
        m_cursorTime = sf::seconds(0);
    }

    m_bgOffset += 32.f*dt.asSeconds();
    if (m_consoleBg1.getTexture())
        m_consoleBg1.setTextureRect(sf::IntRect(m_bgOffset, m_bgOffset, Engine::instance().window().getSize().x, m_conY + (m_drawText.getCharacterSize()/2) - 4));
    if (m_consoleBg2.getTexture())
        m_consoleBg2.setTextureRect(sf::IntRect(-m_bgOffset, m_bgOffset, Engine::instance().window().getSize().x, m_conY + (m_drawText.getCharacterSize()/2) - 4));

    m_borderShape.setSize(sf::Vector2f(Engine::instance().window().getSize().x - 2, (m_conY + (m_drawText.getCharacterSize()/2))- 6));
    m_consoleBg1.setColor(Engine::instance().config().settingColor("con_color", m_defaultConColor));
    m_consoleBg2.setColor(Engine::instance().config().settingColor("con_color", m_defaultConColor));
    m_borderShape.setOutlineColor(Engine::instance().config().settingColor("con_color", m_defaultConColor));
    sf::String tmp = "]" + m_commandString;


    m_commandText.setString(tmp);
    m_commandText.setPosition(2, m_conY - 20);
    if (m_commandText.getFont())
    {
        float glyphW =  2.f;
        if (m_commandString.getSize() > 0)
            glyphW = m_commandText.getFont()->getGlyph(m_commandString[m_cursorPosition], m_commandText.getCharacterSize(), false).bounds.width + 2;
        if (!m_overwrite)
        {
            if (!m_showCursor)
                m_cursorShape.setFillColor(sf::Color::Transparent);
            else
                m_cursorShape.setFillColor(Engine::instance().config().settingColor("con_textcolor", sf::Color::White));
            m_cursorShape.setOutlineColor(sf::Color::Transparent);
            m_cursorShape.setOutlineThickness(0.f);
            m_cursorShape.setSize(sf::Vector2f(2, m_commandText.getCharacterSize()));
        }
        else
        {
            if (!m_showCursor)
                m_cursorShape.setOutlineColor(sf::Color::Transparent);
            else
                m_cursorShape.setOutlineColor(Engine::instance().config().settingColor("con_textcolor", sf::Color::White));
            m_cursorShape.setOutlineThickness(2.f);
            m_cursorShape.setFillColor(sf::Color::Transparent);
            m_cursorShape.setSize(sf::Vector2f(glyphW, m_commandText.getCharacterSize()));
        }

        m_cursorShape.setPosition(((m_commandText.getCharacterSize() / 2) + m_cursorX) + 2, (m_conY - 20)+ 4);
    }
}

void Console::draw(sf::RenderWindow& rt)
{

    if (m_state != Closed)
    {
        m_drawText.setString(Engine::version());
        float versionWidth = (m_drawText.getLocalBounds().width + 2);
        rt.draw(m_consoleBg1);
        rt.draw(m_consoleBg2);

        rt.draw(m_borderShape);
        if (m_state == Opened)
            rt.draw(m_cursorShape);



        if (m_commandText.getFont())
        {
            int sepWidth = m_drawText.getFont()->getGlyph('^', m_drawText.getCharacterSize(), false).advance +
                           m_drawText.getFont()->getGlyph(' ', m_drawText.getCharacterSize(), false).advance;

            if (m_startString != 0)
            {
                sf::String sepString;
                for (int x = 0; x < Engine::instance().window().getSize().x - (versionWidth + sepWidth); x += sepWidth)
                {
                    sepString += " ^";
                }
                m_drawText.setString(sepString);
                m_drawText.setPosition(0, (m_conY- 20) - (m_drawText.getCharacterSize()));
                rt.draw(m_drawText);
            }

            m_drawText.setString(Engine::version());
            m_drawText.setPosition(Engine::instance().window().getSize().x - versionWidth,
                                   (m_conY - 20) - m_drawText.getCharacterSize());
            rt.draw(m_drawText);


            int posY = (m_conY - 20)  - (m_commandText.getCharacterSize() * 2);
            std::vector<LogEntry>::reverse_iterator iter = m_history.rbegin() + m_startString;
            int line = 0;
            for (; iter != m_history.rend(); ++iter)
            {
                if (line > m_maxLines)
                    break;

                switch(((LogEntry)*iter).level)
                {
                    // Info and message use the same color
                    case Message:
                    case Info:
                        m_drawText.setColor(Engine::instance().config().settingColor("con_textcolor", sf::Color::White));
                        break;
                    case Warning:
                        m_drawText.setColor(sf::Color::Yellow);
                        break;
                    case Error:
                        m_drawText.setColor(sf::Color::Red);
                        break;
                    case Fatal:
                        if (!m_showError)
                            m_drawText.setColor(sf::Color::Red);
                        else
                            m_drawText.setColor(sf::Color::Transparent);
                        break;
                }

                m_drawText.setPosition(2, posY);
                m_drawText.setString(">" + ((LogEntry)*iter).message);
                rt.draw(m_drawText);
                posY -= m_drawText.getCharacterSize(); //m_drawText.getCharacterSize() + 8;
                line++;
            }
            m_commandText.setColor(Engine::instance().config().settingColor("con_textcolor", sf::Color::White));

            rt.draw(m_commandText);
        }
    }
}

void Console::print(Console::Level level, const std::string& fmt, ...)
{
    int size = 100;
    std::string str;

    va_list ap;
    while (1)
    {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size)
        {
            str.resize(n);
            break;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }

    std::string label = "";
    std::string timestamp = "";
    std::time_t t = std::time(NULL);
    char mbstr[100];
    if (std::strftime(mbstr, 100, "%H:%M:%S ", std::localtime(&t)))
        timestamp = std::string(mbstr);

    switch(level)
    {
        case Message:
            break;
        case Info:
            label = "[Info] ";
            break;
        case Warning:
            label = "[Warning] ";
            break;
        case Error:
            label = "[Error] ";
            break;
        case Fatal:
            label = "[FATAL] ";
            break;
    }
    std::vector<std::string> entries = zelda::utility::split(str, '\n');

    for (std::string string : entries)
    {
        if (string == std::string())
            break;

        LogEntry entry;
        entry.level = level;
        entry.message = label + string;
        entry.timestamp = timestamp;
        m_history.push_back(entry);
        m_log << timestamp << entry.message.toAnsiString() << std::endl;
        // Increment the start string to prevent
        // the console from scrolling
        if (m_startString > 0)
            m_startString++;

        std::cout << timestamp << entry.message.toAnsiString() << std::endl;
    }



    if (level == Console::Fatal)
    {
        // print to console just incase there is no font
        // and the user can't read it.

        m_hadFatalError = true;
        m_state = Opened;
        m_conHeight = Engine::instance().config().settingInt("vid_height", 480);
        m_conY = m_conHeight;
        recalcMaxLines();
    }
}

bool Console::isInitialized() const
{
    return m_isInitialized;
}

void Console::clear()
{
    m_history.clear();
    m_startString = 0;
}

void Console::resetCursor()
{
    m_commandString = "";
    m_cursorX = 0.0f;
    m_cursorPosition = 0;
    m_currentCommand = 0;
}

void Console::parseCommand()
{
    // If the string is empty don't bother processing it
    if (m_commandString.isEmpty())
        return;

    std::vector<std::string> args = zelda::utility::split(m_commandString.toAnsiString(), ' ');

    m_commandHistory.push_back(m_commandString);

    // If the command history is full
    // Erase the first entry
    if (m_commandHistory.size() > 5)
        m_commandHistory.erase(m_commandHistory.begin());
    print(Message, m_commandString.toAnsiString());

    if (args.size() > 0)
    {
        // Eventually need to handle semi colons for multiple commands
        // The command is always the first word
        std::string command = args[0];
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        // Hard coded commands
        if (!command.compare("list"))
        {
            // List all registered commands
            for (std::pair<std::string, ConsoleCommandBase*> cmd : m_commands)
                print(Message, cmd.first);

            print(Message, "set");
            resetCursor();
            return;
        }

        // remove the command
        args.erase(args.begin());
        if (m_commands.find(command) != m_commands.end())
            m_commands[command]->execute(args);
        else
        {
            if (!command.compare("set"))
            {
                if (args.size() == 0)
                    return;

                std::string setting = args[0];
                zelda::utility::tolower(setting);
                // Not a command?
                // It's probably a setting
                if (args.size() == 2)
                {
                    Engine::instance().config().setSettingLiteral(setting, args[1]);
                }
                else if (args.size() > 1)
                {
                    std::string val = "";
                    args.erase(args.begin());
                    for (std::string s : args)
                    {
                        val += s + " ";
                    }

                    if (val.size() > 1)
                        Engine::instance().config().setSettingLiteral(setting, val);
                }
                else
                    print(Info, "%s -> %s", setting.c_str(), Engine::instance().config().settingLiteral(setting).c_str());
            }
        }
    }

    resetCursor();
}

void Console::recalcMaxLines()
{
    m_maxLines = (m_conHeight / m_drawText.getCharacterSize()) - 3;
}

