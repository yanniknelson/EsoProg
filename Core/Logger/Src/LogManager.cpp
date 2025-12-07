#include "LogManager.h"

#include <stdio.h>

#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Log
{

// Define static members
bool LogManager::m_isInitialized = false;
spdlog::sink_ptr LogManager::m_sharedConsoleSink;
spdlog::sink_ptr LogManager::m_sharedFileSink;
std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> LogManager::m_loggers;
LogLevel LogManager::m_defaultLogLevel = LogLevel::TRACE;
LogLevel LogManager::m_defaultFlushLevel = LogLevel::INFO;

// Constants
const std::string DEFAULT_LOG_PATTERN = "[%n] [%d-%m-%Y %X.%e (%z)] [thread %t] [%s:%# %!] [%^%l%$] %v";
constexpr size_t THREAD_POOL_QUEUE_SIZE = 8192;
constexpr size_t THREAD_POOL_THREADS = 2;
constexpr size_t MAX_LOG_FILE_SIZE_BYTES = 1048576 * 10; // 10 MiB
constexpr size_t MAX_LOG_FILES = 5;

// Init function
void LogManager::Init()
{
    if (!m_isInitialized)
    {
        // --- Initialize thread pool ---
        spdlog::init_thread_pool(THREAD_POOL_QUEUE_SIZE, THREAD_POOL_THREADS);

        // --- Initialize Spdlog's internal error's handler ---
        spdlog::set_error_handler([](const std::string& msg)
        {
            // Fallback for logging internal spdlog errors
            fprintf(stderr, "[SPDLOG ERROR] %s\n", msg.c_str());
        });

        // --- Instantiate the Console Sink (using colored sink) ---
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern(DEFAULT_LOG_PATTERN);
        m_sharedConsoleSink = console_sink;

        // --- Instantiate the File Sink (using rotating file) ---
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/Mixed_Logs.txt", 
            MAX_LOG_FILE_SIZE_BYTES,
            MAX_LOG_FILES
        );
        file_sink->set_pattern(DEFAULT_LOG_PATTERN);
        m_sharedFileSink = file_sink;

        // --- Defaults configurations ---
        spdlog::flush_on(to_spdlog_level(m_defaultFlushLevel));
        spdlog::set_level(to_spdlog_level(m_defaultLogLevel));

        // --- Initialize Default Logger ---
        LogManager::Get("DEFAULT", true, true);
        
        // Mark initialization complete
        m_isInitialized = true;
    }
}

// Shutdown function.
void LogManager::Shutdown()
{
    // Drop all loggers
    spdlog::drop_all();
    // Shutdown spdlog
    spdlog::shutdown();
    // Clear logger cache
    m_loggers.clear();
    // Set initialized as false
    m_isInitialized = false;
}

// Get function.
std::shared_ptr<spdlog::logger> LogManager::Get(const std::string name, bool is_console_output, bool is_file_output, bool is_unqiue_file)
{
    const std::string logger_name = name.empty() ? "DEFAULT" : name; // Use "DEFAULT" constant

    auto it = m_loggers.find(logger_name);
    if (it != m_loggers.end())
    {
        return it->second;
    }

    // Create Sinks for the New Logger
    std::vector<spdlog::sink_ptr> sinks;

    // Console sink
    if (is_console_output && m_sharedConsoleSink)
    {
        sinks.push_back(m_sharedConsoleSink);
    }

    // File sink
    if (is_file_output && m_sharedFileSink)
    {
        sinks.push_back(m_sharedFileSink);
    }

    // If unique file
    if (is_unique_file)
    {
        std::string unique_file_name = spdlog::fmt_lib::format("logs/{}_Logs.txt", logger_name);
        auto unique_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            unique_file_name,
            MAX_LOG_FILE_SIZE_BYTES,
            MAX_LOG_FILES
        );
        unique_file_sink->set_pattern(DEFAULT_LOG_PATTERN);
        sinks.push_back(unique_file_sink);
    }

    // Create new logger (async)
    if (!sinks.empty())
    {
        std::shared_ptr<spdlog::async_logger> new_logger = 
            std::make_shared<spdlog::async_logger>(
                logger_name, 
                sinks.begin(), sinks.end(), 
                spdlog::thread_pool(), 
                spdlog::async_overflow_policy::block
            );

        // Default levels
        new_logger->flush_on(to_spdlog_level(m_defaultFlushLevel));
        new_logger->set_level(to_spdlog_level(m_defaultLogLevel));

        // Cache and Register
        m_loggers[logger_name] = new_logger;
        spdlog::register_logger(new_logger);

        return new_logger;
    }

    // If no sinks were created
    return nullptr;
}

// Drop function.
void LogManager::Drop(const std::string& name)
{
    if (spdlog::get(name))
    {
        spdlog::drop(name);
    }
}

// Setter functions.
void LogManager::SetDefaultLogLevel(const LogLevel& log_level)
{
    m_defaultLogLevel = log_level;
}

void LogManager::SetDefaultFlushLevel(const LogLevel& flush_level)
{
    m_defaultFlushLevel = flush_level;
}

} // namespace Log
