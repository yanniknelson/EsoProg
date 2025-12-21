#pragma once

#include <atomic>
#include <vector>

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
    static bool s_bInitialized;
    static std::atomic<int> s_numLoggers;
};
