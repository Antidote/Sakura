#ifndef CONSOLECOMMANDBASE_HPP
#define CONSOLECOMMANDBASE_HPP


#include <string>
#include <vector>

class ConsoleCommandBase
{
public:
    virtual ~ConsoleCommandBase(){}
    virtual void usage() = 0;
    virtual void execute(std::vector<std::string> args) = 0;
};


// Common Commands

class QuitCommand : public ConsoleCommandBase
{
public:
    QuitCommand() {}
    ~QuitCommand() {}

    void usage();
    void execute(std::vector<std::string> args);
};


// Fullscreen Commands

class FullscreenCommand : public ConsoleCommandBase
{
public:
    FullscreenCommand() {};
    ~FullscreenCommand() {};

    void usage();
    void execute(std::vector<std::string> args);
};

class ClearCommand : public ConsoleCommandBase
{
public:
    ClearCommand() {};
    ~ClearCommand() {};

    void usage();
    void execute(std::vector<std::string> args);
};


class WireframeCommand : public ConsoleCommandBase
{
public:
    WireframeCommand() {};
    ~WireframeCommand() {};

    void usage();
    void execute(std::vector<std::string> args);
};

class PlayCommand : public ConsoleCommandBase
{
public:
    PlayCommand() {};
    ~PlayCommand() {};

    void usage();
    void execute(std::vector<std::string> args);
};

class BindCommand : public ConsoleCommandBase
{
public:
    BindCommand() {};
    ~BindCommand() {};

    void usage();
    void execute(std::vector<std::string> args);
};

class UnbindCommand : public ConsoleCommandBase
{
public:
    UnbindCommand() {};
    ~UnbindCommand() {};

    void usage();
    void execute(std::vector<std::string> args);
};

class RestartCommand : public ConsoleCommandBase
{
public:
    RestartCommand() {};
    ~RestartCommand() {};

    void usage();
    void execute(std::vector<std::string> args);
};

#endif // CONSOLECOMMANDBASE_HPP
