#pragma once

#include "LogLevel.h" // for ELogLevel

#include <memory>
#include <string>

// Forward decl
namespace spdlog { class logger; };
namespace spdlog::sinks { class sink; };

using TSPDLoggerPtr = std::shared_ptr<spdlog::logger>;
using TSinkPtr = std::shared_ptr<spdlog::sinks::sink>;

//////////////////////////////////////////////////////////////
class CLogger
{
  public:
    CLogger(TSPDLoggerPtr pLogger);
    ~CLogger();

    TSPDLoggerPtr& get();

  private:
    TSPDLoggerPtr m_pLogger;
};

using TLoggerPtr = std::shared_ptr<CLogger>;

//////////////////////////////////////////////////////////////
class CLogManager
{
    public:
    static void Init();
    static void Shutdown();

    static TLoggerPtr GetOrCreate(const std::string& name = "", bool bOutputToConsole = true, bool bOutputToFile = true, bool bOutputToUniquewFile = false);
    static void Drop(const std::string& name);

    static void SetDefaultLogLevel(const ELogLevel::Enum& log_level);
    static void SetDefaultFlushLevel(const ELogLevel::Enum& flush_level);

    private:
    /// Private ctor & dtor to enforce singleton pattern.
    CLogManager() = default;
    ~CLogManager() = default;

    /// Delete move/copy ctor and assignment op
    CLogManager(const CLogManager& other) = delete;
    CLogManager(CLogManager&& other) = delete;
    CLogManager& operator=(const CLogManager& other) = delete;
    CLogManager& operator=(CLogManager&& other) = delete;

    static bool s_isInitialized;
    static TSinkPtr s_sharedConsoleSink;
    static TSinkPtr s_sharedFileSink;
    static ELogLevel::Enum s_defaultLogLevel;
    static ELogLevel::Enum s_defaultFlushLevel;
};
