#pragma once

#include <SmartEnums.h>

#include <spdlog/common.h>

// clang-format off
#define LOG_LEVEL(x)\
    x(Trace)\
    x(Debug)\
    x(Info)\
    x(Warn)\
    x(Error)\
    x(Fatal)

    CreateSmartEnum(ELogLevel, LOG_LEVEL)

#undef LOG_LEVEL
// clang-format on

//////////////////////////////////////////////////////////////
inline spdlog::level::level_enum to_spdlog_level(const ELogLevel::Enum& log_level)
{
    switch (log_level)
    {
    case (ELogLevel::Trace):
    {
        return spdlog::level::trace;
    }
    case (ELogLevel::Debug):
    {
        return spdlog::level::debug;
    }
    case (ELogLevel::Info):
    {
        return spdlog::level::info;
    }
    case (ELogLevel::Warn):
    {
        return spdlog::level::warn;
    }
    case (ELogLevel::Error):
    {
        return spdlog::level::err;
    }
    case(ELogLevel::Fatal):
    {
        return spdlog::level::critical;
    }
    default:
    {
        return spdlog::level::info; // invalid defaults to info
    }
    }
};

//////////////////////////////////////////////////////////////
inline ELogLevel::Enum to_level(const spdlog::level::level_enum& spd_level)
{
    switch (spd_level)
    {
        case spdlog::level::trace:    return ELogLevel::Trace;
        case spdlog::level::debug:    return ELogLevel::Debug;
        case spdlog::level::info:     return ELogLevel::Info;
        case spdlog::level::warn:     return ELogLevel::Warn;
        case spdlog::level::err:      return ELogLevel::Error;
        case spdlog::level::critical: return ELogLevel::Fatal;
        default:                      return ELogLevel::COUNT;
    }
}
