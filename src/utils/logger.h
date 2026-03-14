#pragma once
#include "config.h"
#include <string>
#include <iostream>

class Logger {
public:
    static void debug(const std::string& msg);
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);

private:
    static void log(Config::LogLevel level, const std::string& prefix, const std::string& msg);
};
