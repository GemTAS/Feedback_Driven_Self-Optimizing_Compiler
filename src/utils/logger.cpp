#include "logger.h"

void Logger::log(Config::LogLevel level, const std::string& prefix, const std::string& msg) {
    if (level >= Config::currentLogLevel) {
        std::cout << "[" << prefix << "] " << msg << std::endl;
    }
}

void Logger::debug(const std::string& msg) {
    log(Config::LogLevel::DEBUG, "DEBUG", msg);
}

void Logger::info(const std::string& msg) {
    log(Config::LogLevel::INFO, "INFO", msg);
}

void Logger::warn(const std::string& msg) {
    log(Config::LogLevel::WARN, "WARN", msg);
}

void Logger::error(const std::string& msg) {
    log(Config::LogLevel::ERROR, "ERROR", msg);
}
