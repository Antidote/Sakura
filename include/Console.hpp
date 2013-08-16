#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <SFML/Graphics.hpp>
#include "ConsoleCommandBase.hpp"

#include <unordered_map>
#include <fstream>

class Console
{
public:
    enum State
    {
        Opened,
        Closed,
        Opening,
        Closing
    };

    enum Level
    {
        Message,
        Info,
        Warning,
        Error,
        Fatal
    };

    enum
    {
        MAX_LEN = 255
    };

    Console(const std::string& logfile);
    ~Console();

    struct LogEntry
    {
        Level level;
        sf::String timestamp;
        sf::String message;
    };

    void initialize();
    void shutdown();
    bool isOpen() const;

    void handleText(const sf::Uint32 unicode);
    void handleInput(sf::Keyboard::Key code, bool alt, bool control, bool shift, bool system);
    void handleMouseWheel(int delta, int x, int y);


    void update(const sf::Time& dt);
    void draw(sf::RenderWindow& rt);

    void print(Level level, const std::string& fmt, ...);

    bool isInitialized() const;

    void clear();

    void toggleConsole();

private:
    std::vector<std::string> potentialMatches(const std::string& command);
    void parseCommand();
    void resetCursor();
    void recalcMaxLines();

    State m_state;
    bool m_showCursor;
    sf::String m_commandString;
    sf::Text   m_commandText;
    sf::Text  m_drawText;
    sf::Time m_totalWaitTime;
    sf::Time m_waitTime;
    sf::RectangleShape m_borderShape;

    sf::Time m_blinkTimeout;
    sf::Time m_cursorTime;
    sf::Time m_currentTime;
    bool m_showError;

    float      m_bgOffset;
    sf::Sprite m_consoleBg1;
    sf::Sprite m_consoleBg2;
    sf::RectangleShape m_cursorShape;
    int m_cursorPosition;
    int m_startString;
    int m_currentCommand;
    bool m_isInitialized;
    bool m_overwrite;
    bool m_hadFatalError;
    int m_maxLines;
    int m_conHeight;

    float m_cursorX;

    std::ofstream m_log;
    // format is command name -> command class
    // e.g m_commands["quit"] = new QuitCommand();
    std::unordered_map<std::string, ConsoleCommandBase*> m_commands;
    std::vector<LogEntry> m_history;

    // Command history remembers the last 5 commands
    std::vector<sf::String> m_commandHistory;

    float m_conY;

    sf::Color m_defaultConColor;
};

#endif // CONSOLE_HPP
