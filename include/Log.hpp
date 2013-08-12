#ifndef LOG_HPP
#define LOG_HPP
#include <string>
#include <streambuf>
#include <fstream>
#include <teestream.hpp>


class Log
{
public:
    enum Level
    {
        Message,
        Info,
        Warning,
        Error,
        Fatal
    };


    Log(const std::string& logFile);
    ~Log();

    void print(Level level, const std::string& fmt, ...);

private:
    std::ofstream  m_logFile;
    teestream m_logStream;
};

#endif // LOG_HPP
