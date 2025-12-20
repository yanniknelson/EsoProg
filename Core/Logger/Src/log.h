#pragma once

#include "spdlog/spdlog.h"

#include <iostream>

#define DEFAULT_LOGGER_PATTERN "[%n] [%d-%m-%Y %X.%e (%z)] [thread %t] [%s:%# %!] [%^%l%$] %v"

#if defined(PLATFORM_WINDOWS)
#define BREAK __debugbreak();
#elif defined(PLATFORM_MAC)
#define BREAK __builtin_debugtrap();
#elif defined(PLATFORM_LINUX)
#define BREAK __builtin_trap();
#else
#define BREAK __builtin_trap();
#endif

#ifdef DEBUG
#define LOG(loggerName, level, msg, ...)		if (spdlog::get(loggerName)) {spdlog::get(loggerName)->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, fmt::runtime(msg), ##__VA_ARGS__);} else {std::cout << "Tried to log with " << loggerName << " before initializing" << std::endl; BREAK;}
#define LOG_TRACE(loggerName, ...)		LOG(loggerName, spdlog::level::trace, ##__VA_ARGS__)
#define LOG_DEBUG(loggerName, ...)		LOG(loggerName, spdlog::level::debug, ##__VA_ARGS__)
#define LOG_INFO(loggerName, ...)		LOG(loggerName, spdlog::level::info, ##__VA_ARGS__)
#define LOG_WARNING(loggerName, ...)	LOG(loggerName, spdlog::level::warn, ##__VA_ARGS__)
#define LOG_ERROR(loggerName, ...)		LOG(loggerName, spdlog::level::err, ##__VA_ARGS__)
#define LOG_FATAL(loggerName, ...)		do {LOG(loggerName, spdlog::level::critical, ##__VA_ARGS__); BREAK;} while(false)
#else
// Disable all logging for release builds
#define LOG_TRACE(...)		(void)0;
#define LOG_DEBUG(...)		(void)0;
#define LOG_INFO(...)		(void)0;
#define LOG_WARNING(...)	(void)0;
#define LOG_ERROR(...)		(void)0;
#define LOG_FATAL(...)		(void)0;
#endif // !RELEASE
