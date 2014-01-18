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

#ifndef CONSOLECOMMANDBASE_HPP
#define CONSOLECOMMANDBASE_HPP


#include <string>
#include <vector>

namespace Sakura
{
namespace Core
{

typedef void (*commandFunc)(std::vector<std::string>);

class ConsoleCommandBase
{
public:
    //ConsoleCommandBase(const std::string& name, const std::string& help, commandFunc func);
    /*!
     * \brief ~ConsoleCommandBase
     */
    virtual ~ConsoleCommandBase(){}
    /*!
     * \brief usage
     */
    virtual std::string usage() const= 0;
    /*!
     * \brief execute
     * \param args
     */
    virtual void execute(std::vector<std::string> args) = 0;

private:
    std::string m_name;
    std::string m_help;

    commandFunc m_commandFunc;
};


// Common Commands

class QuitCommand : public ConsoleCommandBase
{
public:
    /*!
     * \brief QuitCommand
     */
    QuitCommand() {}
    ~QuitCommand() {}

    /*!
     * \brief usage
     */
    std::string usage() const;

    /*!
     * \brief execute
     * \param args
     */
    void execute(std::vector<std::string> args);
};


// Fullscreen Commands

class FullscreenCommand : public ConsoleCommandBase
{
public:
    /*!
     * \brief FullscreenCommand
     */
    FullscreenCommand() {}

    ~FullscreenCommand() {}

    /*!
     * \brief usage
     */
    std::string usage() const;

    /*!
     * \brief execute
     * \param args
     */
    void execute(std::vector<std::string> args);
};

class ClearCommand : public ConsoleCommandBase
{
public:
    /*!
     * \brief ClearCommand
     */
    ClearCommand() {}
    ~ClearCommand() {}

    /*!
     * \brief usage
     */
    std::string usage() const;
    /*!
     * \brief execute
     * \param args
     */
    void execute(std::vector<std::string> args);
};


class WireframeCommand : public ConsoleCommandBase
{
public:
    /*!
     * \brief WireframeCommand
     */
    WireframeCommand() {}
    ~WireframeCommand() {}

    /*!
     * \brief usage
     */
    std::string usage() const;
    /*!
     * \brief execute
     * \param args
     */
    void execute(std::vector<std::string> args);
};

class PlayCommand : public ConsoleCommandBase
{
public:
    /*!
     * \brief PlayCommand
     */
    PlayCommand() {}
    ~PlayCommand() {}

    /*!
     * \brief usage
     */
    std::string usage() const;
    /*!
     * \brief execute
     * \param args
     */
    void execute(std::vector<std::string> args);
};

class BindCommand : public ConsoleCommandBase
{
public:
    /*!
     * \brief BindCommand
     */
    BindCommand() {}
    ~BindCommand() {}

    /*!
     * \brief usage
     */
    std::string usage() const;

    /*!
     * \brief execute
     * \param args
     */
    void execute(std::vector<std::string> args);
};

class UnbindCommand : public ConsoleCommandBase
{
public:
    /*!
     * \brief UnbindCommand
     */
    UnbindCommand() {}
    ~UnbindCommand() {}

    /*!
     * \brief usage
     */
    std::string usage() const;

    /*!
     * \brief execute
     * \param args
     */
    void execute(std::vector<std::string> args);
};

class RestartCommand : public ConsoleCommandBase
{
public:
    /*!
     * \brief RestartCommand
     */
    RestartCommand() {}
    ~RestartCommand() {}

    /*!
     * \brief usage
     */
    std::string usage() const;

    /*!
     * \brief execute
     * \param args
     */
    void execute(std::vector<std::string> args);
};
} // Core
} // Sakura

#endif // CONSOLECOMMANDBASE_HPP
