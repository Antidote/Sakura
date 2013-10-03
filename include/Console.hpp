#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ConsoleCommandBase.hpp"

#include <unordered_map>
#include <fstream>

class Console
{
public:
    /*!
     * \brief The State enum
     */
    enum State
    {
        Opened,
        Closed,
        Opening,
        Closing
    };

    /*!
     * \brief The Level enum
     */
    enum Level
    {
        Message, //! The lowest level, it is not prepended to the text, usually for chat text, or simple messages that don't need attention
        Info,    //! Usually information about resources, may also contain information relevant to the games state.
        Warning, //! Usually used when the engine can't find a resource, which isn't usually a big deal
        Error,   //! Used when the engine encounters a non-fatal error, like a packet drop
        Fatal    //! Used when the engine encounters a fatal error, like a required resource isn't available, e.g fonts/debug.ttf
    };
    enum
    {
        MAX_LEN = 255 //! The maximum length the command text can be.
    };

    /*!
     * \brief Console
     * \param logfile
     */
    Console(const std::string& logfile);
    ~Console();

    /*!
     * \brief The LogEntry struct
     */
    struct LogEntry
    {
        /*!
         * \brief level
         */
        Level level;
        /*!
         * \brief timestamp
         */
        sf::String timestamp;
        /*!
         * \brief message
         */
        sf::String message;
    };

    /*!
     * \brief initialize Initializes the console back end
     */
    void initialize();

    /*!
     * \brief shutdown Shuts down the console back end clearing resources.
     */
    void shutdown();

    /*!
     * \brief isOpen Returns whether the console is currently open.
     * \return bool True if open, false otherwise.
     */
    bool isOpen() const;

    /*!
     * \brief handleText Used internally to build the command string.
     * \param unicode
     */
    void handleText(const sf::Uint32 unicode);

    /*!
     * \brief handleInput
     * \param code
     * \param alt
     * \param control
     * \param shift
     * \param system
     */
    void handleInput(sf::Keyboard::Key code, bool alt, bool control, bool shift, bool system);

    /*!
     * \brief handleMouseWheel
     * \param delta
     * \param x
     * \param y
     */
    void handleMouseWheel(int delta, int x, int y);


    /*!
     * \brief update
     * \param dt
     */
    void update(const sf::Time& dt);

    /*!
     * \brief draw
     * \param rt
     */
    void draw(sf::RenderWindow& rt);

    /*!
     * \brief print
     * \param level
     * \param fmt
     */
    void print(Level level, const std::string& fmt, ...);

    /*!
     * \brief isInitialized
     * \return
     */
    bool isInitialized() const;

    /*!
     * \brief clear
     */
    void clear();

    /*!
     * \brief toggleConsole
     */
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
    sf::Text   m_drawText;
    sf::Time   m_totalWaitTime;
    sf::Time   m_waitTime;
    sf::RectangleShape m_borderShape;

    sf::Time   m_blinkTimeout;
    sf::Time   m_cursorTime;
    sf::Time   m_currentTime;
    bool       m_showError;

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
    float m_conY;

    sf::Color m_defaultConColor;

    std::ofstream m_log;
    // format is command name -> command class
    // e.g m_commands["quit"] = new QuitCommand();
    std::unordered_map<std::string, ConsoleCommandBase*> m_commands;
    std::vector<LogEntry> m_history;

    // Command history remembers the last 5 commands
    std::vector<sf::String> m_commandHistory;

};

#endif // CONSOLE_HPP
