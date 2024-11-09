#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>

class Logger {

public:

    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    Logger() {
        init();
    }

    ~Logger() {
        shutdown();
    }

    template <typename... Args>
    void log(LogLevel level, const std::string& message, Args... args) {
        if (!isEnabled(level)) {
            return;
        }

        std::ostringstream out_message;
        out_message << getColorCode(level) << "[" << getLevelString(level) << "] " << message;
        ((out_message << " " << toString(args)), ...);

        const std::string out_message_str = out_message.str() + "\033[0m"; 

        if (level == LogLevel::ERROR || level == LogLevel::FATAL) {
            std::cerr << out_message_str << std::endl;
        } else {
            std::cout << out_message_str << std::endl;
        }
    }

    template <typename... Args>
    void fatal(const std::string& message, const Args&... args) {
        log(LogLevel::FATAL, message, args...);
    }

    template <typename... Args>
    void error(const std::string& message, const Args&... args) {
        log(LogLevel::ERROR, message, args...);
    }

    template <typename... Args>
    void warning(const std::string& message, const Args&... args) {
        log(LogLevel::WARNING, message, args...);
    }

    template <typename... Args>
    void info(const std::string& message, const Args&... args) {
        log(LogLevel::INFO, message, args...);
    }

    template <typename... Args>
    void debug(const std::string& message, const Args&... args) {
        log(LogLevel::DEBUG, message, args...);
    }

    template <typename... Args>
    void trace(const std::string& message, const Args&... args) {
        log(LogLevel::TRACE, message, args...);
    }

    void assertFailure(const char* expression, const char* message, const char* file, int line) {
        log(LogLevel::FATAL, "Assertion failed: " + std::string(expression) + " in file " + std::string(file) + " in line " + std::to_string(line), message);
        abort();
    }

private:
    bool isEnabled(LogLevel level) const {
        return (level == LogLevel::FATAL) ||
               (level == LogLevel::ERROR) ||
               (level == LogLevel::WARNING && isWarningEnabled()) ||
               (level == LogLevel::INFO && isInfoEnabled()) ||
               (level == LogLevel::DEBUG && isDebugEnabled()) ||
               (level == LogLevel::TRACE && isTraceEnabled());
    }

    bool isWarningEnabled() const {
#ifdef LOG_WARNING_ENABLED
        return true;
#else
        return false;
#endif
    }

    bool isInfoEnabled() const {
#define LOG_INFO_ENABLED 1
#ifdef LOG_INFO_ENABLED
        return true;
#else
        return false;
#endif
    }

    bool isDebugEnabled() const {
#ifdef LOG_DEBUG_ENABLED
        return true;
#else
        return false;
#endif
    }

    bool isTraceEnabled() const {
#ifdef LOG_TRACE_ENABLED
        return true;
#else
        return false;
#endif
    }

    const char* getLevelString(LogLevel level) const {
        switch (level) {
            case LogLevel::TRACE:
                return "TRACE";
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARNING:
                return "WARNING";
            case LogLevel::ERROR:
                return "ERROR";
            case LogLevel::FATAL:
                return "FATAL";
            default:
                return "UNKNOWN";
        }
    }

    const char* getColorCode(LogLevel level) const {
        switch (level) {
            case LogLevel::TRACE:
                return "\033[37m";
            case LogLevel::DEBUG:
                return "\033[36m";
            case LogLevel::INFO:
                return "\033[32m";
            case LogLevel::WARNING:
                return "\033[33m";
            case LogLevel::ERROR:
                return "\033[31m";
            case LogLevel::FATAL:
                return "\033[1;31m";
            default:
                return "\033[0m";
        }
    }

    template <typename T>
    std::string toString(const T& value) const {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    void init() {
    }

    void shutdown() {
    }
};