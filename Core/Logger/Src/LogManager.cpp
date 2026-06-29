#include "LogManager.h"

#include "LogLevel.h" // for ELogLevel

#include <spdlog/async.h>                    // for spdlog::init_thread_pool
#include <spdlog/async_logger.h>             // for spdlog::async_logger
#include <spdlog/fmt/bundled/format.h>       // for spdlog::fmt_lib::format
#include <spdlog/logger.h>                   // for spdlog::logger
#include <spdlog/sinks/rotating_file_sink.h> // for spdlog::sinks::basic_file_sink_mt
#include <spdlog/sinks/stdout_color_sinks.h> // for spdlog::sinks::stderr_color_sink_mt
// clang-format off
// spdlog.h must be included before spdlog-inl.h
#include <spdlog/spdlog.h>
#include <spdlog/spdlog-inl.h>                // for spdlog::get
// clang-format on

#include <cstdio> // for stderr
#include <memory>
#include <string>
#include <vector>

// Define static members
bool CLogManager::s_isInitialized = false;
TSinkPtr CLogManager::s_sharedConsoleSink;
TSinkPtr CLogManager::s_sharedFileSink;
ELogLevel::Enum CLogManager::s_defaultLogLevel = ELogLevel::Trace;
ELogLevel::Enum CLogManager::s_defaultFlushLevel = ELogLevel::Warn;
ETraceVerbosityLevel::Enum CLogManager::s_verbosityLevel = ETraceVerbosityLevel::Off;

// Constants
const std::string DEFAULT_LOG_PATTERN = "[%n] [%d-%m-%Y %X] [thread %t] [%!] [%^%l%$] %v";
constexpr size_t THREAD_POOL_QUEUE_SIZE = 1024 * 1024;
constexpr size_t THREAD_POOL_THREADS = 4;

constexpr size_t MAX_LOG_FILE_SIZE_BYTES = 1048576 * 10; // 10 MiB
constexpr size_t MAX_LOG_FILES = 5;

//////////////////////////////////////////////////////////////
CLogger::CLogger(TSPDLoggerPtr pLogger)
    : m_pLogger(pLogger)
{
}

//////////////////////////////////////////////////////////////
CLogger ::~CLogger()
{
    CLogManager::Drop(m_pLogger->name().c_str());
}

//////////////////////////////////////////////////////////////
TSPDLoggerPtr& CLogger::get()
{
    return m_pLogger;
}

//////////////////////////////////////////////////////////////
void CLogManager::Init()
{
    if (!s_isInitialized)
    {
        spdlog::init_thread_pool(THREAD_POOL_QUEUE_SIZE, THREAD_POOL_THREADS);
        spdlog::set_error_handler([](const std::string& msg)
            { fprintf(stderr, "[SPDLOG ERROR] %s\n", msg.c_str()); });

        TSinkPtr console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern(DEFAULT_LOG_PATTERN);
        s_sharedConsoleSink = console_sink;
        TSinkPtr file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/Mixed_Logs.txt", MAX_LOG_FILE_SIZE_BYTES, MAX_LOG_FILES);
        file_sink->set_pattern(DEFAULT_LOG_PATTERN);
        s_sharedFileSink = file_sink;
        spdlog::flush_on(to_spdlog_level(s_defaultFlushLevel));
        spdlog::set_level(to_spdlog_level(s_defaultLogLevel));
        CLogManager::GetOrCreate("DEFAULT", true, true);
        s_isInitialized = true;
    }
}

//////////////////////////////////////////////////////////////
void CLogManager::Shutdown()
{
    spdlog::drop_all();
    spdlog::shutdown();
    s_isInitialized = false;
}

//////////////////////////////////////////////////////////////
TLoggerPtr CLogManager::GetOrCreate(const std::string& name /*= ""*/, bool bOutputToConsole /*= true*/, bool bOutputToFile /*= true*/, bool bOutputToUniqueFile /*= false*/)
{
    if (!s_sharedConsoleSink)
    {
        Init();
    }

    const std::string logger_name = name.empty() ? "DEFAULT" : name; // Use "DEFAULT" constant

    TSPDLoggerPtr logger = spdlog::get(logger_name);
    if (logger)
    {
        return std::make_shared<CLogger>(logger);
    }

    std::vector<TSinkPtr> sinks;

    if (bOutputToConsole && s_sharedConsoleSink)
    {
        sinks.push_back(s_sharedConsoleSink);
    }

    if (bOutputToFile && s_sharedFileSink)
    {
        sinks.push_back(s_sharedFileSink);
    }

    if (bOutputToUniqueFile)
    {
        std::string unique_file_name = spdlog::fmt_lib::format("logs/{}_Logs.txt", logger_name);
        TSinkPtr unique_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(unique_file_name, MAX_LOG_FILE_SIZE_BYTES, MAX_LOG_FILES);
        unique_file_sink->set_pattern(DEFAULT_LOG_PATTERN);
        sinks.push_back(unique_file_sink);
    }

    // Create new logger (async)
    if (!sinks.empty())
    {
        std::shared_ptr<spdlog::async_logger> new_logger = std::make_shared<spdlog::async_logger>(logger_name, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

        new_logger->flush_on(to_spdlog_level(s_defaultFlushLevel));
        new_logger->set_level(to_spdlog_level(s_defaultLogLevel));

        spdlog::register_logger(new_logger);

        return std::make_shared<CLogger>(new_logger);
        }

    return nullptr;
}

//////////////////////////////////////////////////////////////
void CLogManager::Drop(const std::string& name)
{
    if (spdlog::get(name))
    {
        spdlog::drop(name);
    }
}

//////////////////////////////////////////////////////////////
void CLogManager::SetDefaultLogLevel(const ELogLevel::Enum& logLevel)
{
    s_defaultLogLevel = logLevel;
    spdlog::apply_all([logLevel](TSPDLoggerPtr logger)
        { logger->set_level(to_spdlog_level(logLevel)); });
}

//////////////////////////////////////////////////////////////
void CLogManager::SetDefaultFlushLevel(const ELogLevel::Enum& flushLevel)
{
    s_defaultFlushLevel = flushLevel;
    spdlog::apply_all([flushLevel](TSPDLoggerPtr logger)
        { logger->flush_on(to_spdlog_level(flushLevel)); });
}

//////////////////////////////////////////////////////////////
void CLogManager::SetTraceVerbosity(const ETraceVerbosityLevel::Enum& verbosityLevel)
{
    s_verbosityLevel = verbosityLevel;
}

//////////////////////////////////////////////////////////////
ETraceVerbosityLevel::Enum CLogManager::GetTraceVerbosity()
{
    return s_verbosityLevel;
}
