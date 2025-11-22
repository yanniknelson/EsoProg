#pragma once

#include <memory>
#include <vector>
#include <atomic>

///////////////////////////////////////////
class CLogManager
{
public:
	struct SLoggerDetails
	{
		const char* m_loggerName{ "" };
		bool m_bConsoleOutput{ true };
		bool m_bFileOutput{ true };
	};

	static void Initialize(const std::vector<SLoggerDetails>& rLoggers);
	static void Shutdown(const char* m_loggerName);

private:
	static bool m_bInitialized;
	static std::atomic<int> m_nLoggers;
};