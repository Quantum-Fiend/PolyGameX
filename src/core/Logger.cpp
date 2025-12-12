#include "PolyGameX/Core/Logger.h"
#include <chrono>
#include <iomanip>

namespace PolyGameX {

LogLevel Logger::s_LogLevel = LogLevel::Info;
std::ofstream Logger::s_LogFile;
bool Logger::s_FileLoggingEnabled = false;

void Logger::Info(const std::string& message) {
    Log(LogLevel::Info, message);
}

void Logger::Warning(const std::string& message) {
    Log(LogLevel::Warning, message);
}

void Logger::Error(const std::string& message) {
    Log(LogLevel::Error, message);
}

void Logger::Debug(const std::string& message) {
    Log(LogLevel::Debug, message);
}

void Logger::EnableFileLogging(const std::string& filename) {
    s_LogFile.open(filename, std::ios::out | std::ios::app);
    if (s_LogFile.is_open()) {
        s_FileLoggingEnabled = true;
        Info("File logging enabled: " + filename);
    }
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (level < s_LogLevel) return;
    
    std::string timestamp = GetTimestamp();
    std::string levelStr = LevelToString(level);
    std::string fullMessage = "[" + timestamp + "] [" + levelStr + "] " + message;
    
    // Console output with colors
    switch (level) {
        case LogLevel::Info:
            std::cout << "\033[32m" << fullMessage << "\033[0m" << std::endl;
            break;
        case LogLevel::Warning:
            std::cout << "\033[33m" << fullMessage << "\033[0m" << std::endl;
            break;
        case LogLevel::Error:
            std::cerr << "\033[31m" << fullMessage << "\033[0m" << std::endl;
            break;
        case LogLevel::Debug:
            std::cout << "\033[36m" << fullMessage << "\033[0m" << std::endl;
            break;
    }
    
    // File output
    if (s_FileLoggingEnabled && s_LogFile.is_open()) {
        s_LogFile << fullMessage << std::endl;
        s_LogFile.flush();
    }
}

std::string Logger::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Debug: return "DEBUG";
        default: return "UNKNOWN";
    }
}

} // namespace PolyGameX
