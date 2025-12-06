#include "LogLifecycle.h"

#include "spdlog/spdlog.h"
#include "LogTags.h"
#include "LogManager.h"

#include <vector>

///////////////////////////////////////////
void InitializeLoggingSystem()
{
    std::vector<CLogManager::SLoggerDetails> loggers =
    {
        { Logger::CORE, true, true, spdlog::level::trace },
        { Logger::APP,  true, true, spdlog::level::trace },
        { Logger::PIET, true, true, spdlog::level::trace },
        { Logger::COW,  true, true, spdlog::level::trace },
        { Logger::AUDL, true, true, spdlog::level::trace },
        { Logger::BRAINFUCK, true, true, spdlog::level::trace }
    };

    CLogManager::Initialize(loggers);
}

///////////////////////////////////////////
void ShutdownLoggingSystem()
{
    CLogManager::ShutdownAll();
}
