#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>

#include "LogLevel.h"

// Forward declaration of the spdlog logger type
namespace spdlog
{

class logger;
using TLoggerPtr = std::shared_ptr<logger>;

namespace sinks
{
    template<typename Mutex> class base_sink;
    using TSinkPtr = std::shared_ptr<base_sink<std::mutex>>;
}
}

/**
 * @namespace Log
 * @brief Logging-specific classes, and functions.
 */
namespace Log
{

/**
 * @brief Log manager class.
 */
class CLogManager
{
public:
    /// Routine Functions.

    /**
     * @brief Centralized initialization routine for the entire logging system.
     * This must be called once at application startup.
     */
    static void Init();

    /**
     * @brief Centralized deinitialization routine for the entire logging system.
     * This must be called once at application termination.
     */
    static void Shutdown();

    /// Getter and Dropper functions

    /**
     * @brief Retrieves a named logger instance. Creates it if it doesn't exist.
     *
     * @param[in] name The unique name of the logger (e.g., "graphics", "core").
     * @param[in] is_console_output Whether to do console output or not.
     * @param[in] is_file_output Whether to do file output or not.
     *
     * @returns A shared pointer to the requested spdlog logger.
     */
    static TLoggerPtr Get(const std::string& name = "", bool is_console_output = true, bool is_file_output = true, bool is_unique_file = false);

    /**
     * @brief Drops a logger instance.
     *
     * @param[in] name The unique name of the already created logger.
     */
    static void Drop(const std::string& name);

    /// Setting Default functions

    /**
     * @brief Sets default log level.
     *
     * @param[in] log_level The new default log level.
     */
    static void SetDefaultLogLevel(const LogLevel& log_level);

    /**
     * @brief Sets default flush level.
     *
     * @param[in] flush_level The new default flush level.
     */
    static void SetDefaultFlushLevel(const LogLevel& flush_level);

private:

    /**
     * @brief Private ctor to enforce singleton pattern.
     */
    CLogManager() = default;
    
    /// Whether the logger manager is initialized or not.
    static bool m_isInitialized;

    /// Holds all initialized loggers, mapping name to the spdlog object.
    static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_loggers;

    /// A pointer to the console sink.
    static spdlog::sinks::TSinkPtr m_sharedConsoleSink;

    /// A pointer to the file sink.
    static spdlog::sinks::TSinkPtr m_sharedFileSink;

    /// Default Log LogLevel
    static LogLevel m_defaultLogLevel;

    /// Default Flush LogLevel
    static LogLevel m_defaultFlushLevel;
};
