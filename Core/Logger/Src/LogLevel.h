#pragma once

#include <cctype>
#include <cstdint>
#include <string>

#include "spdlog/spdlog.h"

namespace {

/**
 * @brief Function to convert a given string to lowercase.
 *
 * @param[in] str The given string.
 *
 * @returns Lowercase string.
 */
std::string to_lowercase(const std::string& str)
{
    std::string temp = str;
    for (char& c : temp)
    {
        // C++ standard requires that the argument given to
        // std::tolower must be convertible to unsigned char.
        c = static_cast<char>(
                std::tolower(static_cast<unsigned char>(c)));
    }
    return temp;
}

} // namespace anonymous

/**
 * @namespace Log
 * @brief Contains logging-specific classes and functions.
 */
namespace Log {

/**
 * @brief Enum class to hold log levels.
 */
enum class LogLevel : std::uint8_t
{
    /**
     * @brief Trace log level.
     * Enum to denote log level of function's work trace (fine-grain information).
     */
    TRACE = 0x00,

    /**
     * @brief Debug log level.
     * Enum to denote debug information of program (debug information).
     */
    DEBUG = 0x01,

    /**
     * @brief Info log level.
     * Enum to denote information about program's state (general information).
     */
    INFO = 0x02,


    /**
     * @brief Warn log level.
     * Enum to denote warning in program state (potentially harmful).
     */
    WARN = 0x03,

    /**
     * @brief Error log level.
     * Enum to denote that an error has occurred (always harmful).
     */
    ERROR = 0x04,

    /**
     * @brief Fatal log level.
     * Enum to denote something critical has been compromised (most harmful, leads to abort).
     */
    FATAL = 0x05,

    /**
     * @brief Unknown log level.
     * Enum to denote catch-all unknown log levels.
     */
    UNKNOWN = 0xFF
        
}; // enum class LogLogLevel

/**
 * @brief Function to convert std::string into log level.
 * This function performs simple look-up to convert std::string into
 * corresponding log level.
 *
 * @param[in] str The given string.
 *
 * @returns Corresponding log level.
 */
[[nodiscard]] inline LogLevel from_string(const std::string& str)
{
    // Convert to lowercase for case-agnostic comparision.
    std::string str_lower = to_lowercase(str);

    if (str_lower == "trace")
    {
        return LogLevel::TRACE;
    }
    else if (str_lower == "debug")
    {
        return LogLevel::DEBUG;
    }
    else if (str_lower == "info")
    {
        return LogLevel::INFO;
    }
    else if (str_lower == "warn")
    {
        return LogLevel::WARN;
    }
    else if (str_lower == "error")
    {
        return LogLevel::ERROR;
    }
    else if (str_lower == "fatal")
    {
        return LogLevel::FATAL;
    }
    else
    {
        return LogLevel::UNKNOWN;
    }
}

/**
 * @brief Function to convert log level into string.
 * This function performs simple look-up to convert LogLevel into
 * corresponding string.
 *
 * @param[in] log_level The given log level.
 *
 * @returns Corresponding string.
 */
[[nodiscard]] inline std::string to_string(const LogLevel& log_level)
{
    switch (log_level)
    {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default:           return "UNKNOWN";
    }
}

/**
 * @brief Maps LogLevel to spdlog levels.
 *
 * @param[in] log_level The log level.
 *
 * @returns Corresponding spdlog level.
 */
[[nodiscard]] inline spdlog::level::level_enum to_spdlog_level(const LogLevel& log_level)
{
    switch (log_level)
    {
        case LogLevel::TRACE: return spdlog::level::trace;
        case LogLevel::DEBUG: return spdlog::level::debug;
        case LogLevel::INFO:  return spdlog::level::info;
        case LogLevel::WARN:  return spdlog::level::warn;
        case LogLevel::ERROR: return spdlog::level::err;
        case LogLevel::FATAL: return spdlog::level::critical;
        default:           return spdlog::level::info; // Optionated default
    }
}

/**
 * @brief Maps spdlog levels to LogLevel.
 *
 * @param[in] spd_level The spdlog level.
 *
 * @returns Corresponding LogLevel.
 */
[[nodiscard]] inline LogLevel to_level(const spdlog::level::level_enum& spd_level)
{
    switch (spd_level)
    {
        case spdlog::level::trace:    return LogLevel::TRACE;
        case spdlog::level::debug:    return LogLevel::DEBUG;
        case spdlog::level::info:     return LogLevel::INFO;
        case spdlog::level::warn:     return LogLevel::WARN;
        case spdlog::level::err:      return LogLevel::ERROR;
        case spdlog::level::critical: return LogLevel::FATAL;
        default:                      return LogLevel::UNKNOWN;
    }
}

} // namespace Log
