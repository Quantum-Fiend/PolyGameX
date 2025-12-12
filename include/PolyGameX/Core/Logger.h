#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolyGameX {

enum class LogLevel {
    Info,
    Warning,
    Error,
    Debug
};

class Logger {
public:
    static void Info(const std::string& message);
    static void Warning(const std::string& message);
    static void Error(const std::string& message);
    static void Debug(const std::string& message);
    
    static void SetLogLevel(LogLevel level) { s_LogLevel = level; }
    static void EnableFileLogging(const std::string& filename);
    
private:
    static void Log(LogLevel level, const std::string& message);
    static std::string GetTimestamp();
    static std::string LevelToString(LogLevel level);
    
    static LogLevel s_LogLevel;
    static std::ofstream s_LogFile;
    static bool s_FileLoggingEnabled;
};

} // namespace PolyGameX
