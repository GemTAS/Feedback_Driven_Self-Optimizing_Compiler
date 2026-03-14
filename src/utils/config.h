#pragma once

namespace Config {
    // Optimization thresholds
    extern int unrollFactor;      // Default: 4
    extern int hotThreshold;      // Default: 10
    extern int extremeHotThreshold; // Default: 1000
    extern int extremeUnrollFactor; // Default: 8

    // Logging configuration
    enum class LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
        NONE = 4
    };

    extern LogLevel currentLogLevel;
}
