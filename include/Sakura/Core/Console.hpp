// This file is part of Sakura.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura.  If not, see <http://www.gnu.org/licenses/>

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ConsoleCommandBase.hpp"

#include <unordered_map>
#include <fstream>

namespace Sakura
{
namespace Core
{
/*!
 * \class Console
 * \brief The Console is a useful debugging utility, it provides a log, and the ability to execute simple commands.
 *
 * The goal of the Console is to provide a Quake like drop console to perform tasks that would otherwise
 * Require the user to close the game and edit the configuration title.
 * Currently the Console isn't very robust and needs to be improved. <br/>
 * As well as provide a way to print messages that can be viewed by the user, and used in a bug report.
 * Printing to the console is relatively easy, you simply need to request the console's instance from the Engine
 * and use the Console class' #print function.
 *
 * \section console_example Printing
 * Using a reference:
 * \code
 *  sEngineRef().console().print(Sakura::Core::Console::Message, "Message to print");
 * \endcode
 * Using a pointer
 * \code
 *  sEnginePtr()->console().print(Sakura::Core::Console::Message, "Message to print")
 * \endcode
 *
 * \todo Improve the interface
 */
class Console
{
public:
    /*!
     * \enum State
     * \brief The State enumerator is used to determine the Console's current state.
     */
    enum State
    {
        Opened,  //!< The Console is open and accepting input
        Closed,  //!< The Console is closed and not drawn
        Opening, //!< The Console is in the process of opening, but not accepting input
        Closing  //!< The Console is in the process of closing, and is still drawn
    };

    /*!
     * \brief The Level enum
     */
    enum Level
    {
        Message, //!< The lowest level, it is not prepended to the text, usually for chat text, or simple messages that don't need attention
        Info,    //!< Usually information about resources, may also contain information relevant to the games state.
        Warning, //!< Usually used when the engine can't find a resource, which isn't usually a big deal
        Error,   //!< Used when the engine encounters a non-fatal error, like a packet drop
        Fatal    //!< Used when the engine encounters a fatal error, like a required resource isn't available, e.g fonts/debug.ttf
    };
    enum
    {
        MAX_LEN = 255 //! The maximum length the command text can be.
    };

    /*!
     * \brief Console
     * \param logfile
     */
    Console(const std::string& logfile="log.txt");
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
     * \brief Handles mouse wheel events.
     * \param delta The amount of ticks <0 down >0 up 0 none
     * \param x The Cursor's x Coord relative to the top left corner of the Window
     * \param y The Cursor's y Coord relative to the top left corner of the Window
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
     * \brief A simple c-styled formatted print.
     *
     * Prints a specified string to both the logfile and to the Console's history,
     * which is then drawn on the screen.
     *
     * \param level The Level of the message
     * \param fmt The formatted string to print
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

    /*!
     * \brief registerCommand
     * \param commandString
     * \param commandClass
     */
    void registerCommand(const std::string& commandString, ConsoleCommandBase* commandClass);

    /*!
     * \brief consoleColor
     * \return
     */
    sf::Color consoleColor() const;

    /*!
     * \brief textColor
     * \return
     */
    sf::Color textColor() const;

private:
    //! \internal
    void doAutoComplete();
    void drawHistory(sf::RenderWindow& rt);
    void drawSeparator(sf::RenderWindow& rt);
    void drawVersion(sf::RenderWindow& rt);
    void parseCommand();
    void resetCursor();
    void recalcMaxLines();

    State      m_state;
    bool       m_showCursor;
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

    float              m_bgOffset;
    sf::Sprite         m_consoleBg1;
    sf::Sprite         m_consoleBg2;
    sf::RectangleShape m_cursorShape;
    int                m_cursorPosition;
    int                m_startString;
    int                m_currentCommand;
    bool               m_isInitialized;
    bool               m_overwrite;
    bool               m_hadFatalError;
    int                m_maxLines;
    int                m_conHeight;

    float              m_cursorX;
    float              m_conY;

    sf::Color m_defaultConColor;

    std::ofstream m_log;
    // format is command name -> command class
    // e.g m_commands["quit"] = new QuitCommand();
    std::unordered_map<std::string, ConsoleCommandBase*> m_commands;
    std::vector<LogEntry> m_history;

    // Command history remembers the last 5 commands
    std::vector<sf::String> m_commandHistory;

    sf::Text m_fitText;
};

} // Core
} // Sakura
#endif // CONSOLE_HPP
