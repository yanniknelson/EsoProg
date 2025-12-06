#include "LogManager.h"

#include "log.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <iostream>
#include <memory>

bool CLogManager::m_bInitialized = false;
std::atomic<int> CLogManager::m_nLoggers = 0;


///////////////////////////////////////////
void CLogManager::Initialize(const std::vector<SLoggerDetails>& rLoggers)
{
	if (!m_bInitialized)
	{
		spdlog::init_thread_pool(8192, 2);
		m_bInitialized = true;
	}
	for (const SLoggerDetails& rLoggerDetail : rLoggers)
	{
		if (!spdlog::get(rLoggerDetail.m_loggerName))
		{
			std::vector<spdlog::sink_ptr> sinks;
			if (rLoggerDetail.m_bConsoleOutput)
			{
				std::shared_ptr<spdlog::sinks::stderr_color_sink_mt> pConsoleSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
				pConsoleSink->set_pattern(DEFAULT_LOGGER_PATTERN);
				sinks.push_back(pConsoleSink);
			}
			if (rLoggerDetail.m_bFileOutput)
			{
				std::shared_ptr<spdlog::sinks::basic_file_sink_mt> pUniqueFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(spdlog::fmt_lib::format("logs/{}_Logs.txt", rLoggerDetail.m_loggerName), true);
				
				pUniqueFileSink->set_pattern(DEFAULT_LOGGER_PATTERN);
				sinks.push_back(pUniqueFileSink);
			}
			std::shared_ptr<spdlog::sinks::basic_file_sink_mt> pSharedFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Mixed_Logs.txt", true);
			pSharedFileSink->set_pattern(DEFAULT_LOGGER_PATTERN);
			sinks.push_back(pSharedFileSink);
			if (!sinks.empty())
			{
				std::shared_ptr<spdlog::async_logger> pLogger = std::make_shared<spdlog::async_logger>(rLoggerDetail.m_loggerName, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
				pLogger->set_level(rLoggerDetail.m_level);
				pLogger->flush_on(rLoggerDetail.m_level);
				spdlog::register_logger(pLogger);
				m_nLoggers += 1;
			}
		}
		else
		{
			LOG_WARNING(rLoggerDetail.m_loggerName, "Tried to create logger {} twice!", rLoggerDetail.m_loggerName);
		}
	}
}

///////////////////////////////////////////
void CLogManager::Shutdown(const char* m_loggerName)
{
	if (spdlog::get(m_loggerName))
	{
		spdlog::drop(m_loggerName);
		if (!--m_nLoggers)
		{
			m_bInitialized = false;
			spdlog::shutdown();
		}
	}
}

///////////////////////////////////////////
void CLogManager::ShutdownAll()
{
    spdlog::drop_all();
    m_bInitialized = false;
    m_nLoggers = 0;
    spdlog::shutdown();
}
