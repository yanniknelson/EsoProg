#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/format.h"

// Available in all builds
#define LOG(logger, log_level, ...) \
    do { \
        if (logger && logger->should_log(log_level)) { \
            logger->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, \
            log_level, \
            __VA_ARGS__); \
        } \
    } while (0)

#define LOG_INFO(logger, ...)    LOG(logger, spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(logger, ...)    LOG(logger, spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(logger, ...)   LOG(logger, spdlog::level::err, __VA_ARGS__)
#define LOG_FATAL(logger, ...) \
    do { \
        if (logger) { \
            logger->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, \
                        spdlog::level::critical, \
                        __VA_ARGS__); \
            spdlog::shutdown(); \
            std::abort(); \
        } \
    } while (0)


// Available in debug builds
#if defined(DEBUG)

# define LOG_TRACE(logger, ...)   LOG(logger, spdlog::level::trace, __VA_ARGS__)
# define LOG_DEBUG(logger, ...)   LOG(logger, spdlog::level::debug, __VA_ARGS__)

#else

# define LOG_TRACE(logger, ...)      (void)(0)
# define LOG_DEBUG(logger, ...)      (void)(0)

#endif // defined(DEBUG)

// Assertion macro
#define LOG_ASSERT(logger, condition, ...) \
    do { \
        if (!(condition)) { \
            LOG_FATAL(logger, "ASSERTION FAILED: {}. Additional context: {}", #condition, spdlog::fmt_lib::format(__VA_ARGS__)); \
        } \
    } while (0)
