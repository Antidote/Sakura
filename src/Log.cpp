#include "Log.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>

#ifdef __WIN32
#include <stdlib.h> // for exit
#include <windows.h>


enum {
    BLACK             = 0,
    DARKBLUE          = FOREGROUND_BLUE,
    DARKGREEN         = FOREGROUND_GREEN,
    DARKCYAN          = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKRED           = FOREGROUND_RED,
    DARKMAGENTA       = FOREGROUND_RED | FOREGROUND_BLUE,
    DARKYELLOW        = FOREGROUND_RED | FOREGROUND_GREEN,
    DARKGRAY          = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GRAY              = FOREGROUND_INTENSITY,
    BLUE              = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN             = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN              = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED               = FOREGROUND_INTENSITY | FOREGROUND_RED,
    MAGENTA           = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW            = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE             = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
};

#endif
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <SFML/Config.hpp>

Log::Log(const std::string& logFile)
    : m_logFile(logFile),
      m_logStream(std::cout, m_logFile)
{
    print(Log::Info, "Log initialized\n");
}

Log::~Log()
{
    print(Log::Info, "Log shutdown\n");
}

void Log::print(Log::Level level, const std::string& fmt, ...)
{
#ifdef SFML_SYSTEM_WINDOWS
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
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
#ifdef SFML_SYSTEM_LINUX
    std::string color = "";
#endif
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
#ifdef SFML_SYSTEM_WINDOWS
            SetConsoleTextAttribute(hConsole, WHITE);
#else
            color = "\x1B[1;37m";
#endif
            label = "[Info] ";
            break;
        case Warning:
#ifdef SFML_SYSTEM_WINDOWS
            SetConsoleTextAttribute(hConsole, YELLOW);
#else
            color = "\x1B[1;33m";
#endif
            label = "[Warning] ";
            break;
        case Error:
#ifdef SFML_SYSTEM_WINDOWS
            SetConsoleTextAttribute(hConsole, RED);
#else
            color = "\x1B[1;31m";
#endif
            label = "[Error] ";
            break;
        case Fatal:
#ifdef SFML_SYSTEM_WINDOWS
            SetConsoleTextAttribute(hConsole, RED);
#else
            color = "\x1B[5;31m";
#endif
            label = "[FATAL] ";
            break;
    }
#ifdef SFML_SYSTEM_WINDOWS
    m_logStream << ((level != Log::Message) ? timestamp  : "") << label << str;
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
#else
    m_logStream << color << ((level != Log::Message) ? timestamp  : "") << label << timestamp << " "<< str <<  "\x1B[0m";
#endif

    if (level == Log::Fatal)
        exit(1);
}
