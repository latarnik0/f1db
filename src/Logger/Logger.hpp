#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger
{
public:
    Logger(const std::string& filename);
    ~Logger();
    static Logger& getInstance();
    void log(LogLevel level, const std::string& message);

private:
    std::ofstream logFile;
    std::string levelToString(LogLevel level);

    // delete copy and assignment operators - logger is only accesivle via getInstance()
    Logger(const Logger&) = delete; 
    Logger& operator=(const Logger&) = delete;
};