#pragma once

#include <ELanguages.h>

#include <iostream>
#include <sstream>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>

struct RuntimeSyncronisationStruct
{
    std::atomic<int> iterations = 0;
    std::atomic<int> instructionWaitTime = 0;
    std::atomic<bool> exit = false;
    std::atomic<bool> renderWantsState = false;
    std::atomic<bool> wantsReset = false;
    std::condition_variable finishedStateWithCv;
    std::mutex finishedWithStateMtx;
    std::mutex runtimeStateMtx;

    std::condition_variable waitingOnInputCV;
    std::mutex waitingOnInputMtx;
};

class IRuntime
{
  public:
    IRuntime(RuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : m_rSync(rSync), m_rOutputStream(rOutputStream), m_rExecutionHistoryStream(rExecutionhistoryStream)
    {
    }

    virtual ELanguages::Enum GetRuntimeLanguage() const = 0;
    virtual std::vector<std::string> GetSupportedFileTypes() const = 0;

    virtual void SetSourceCode(std::string str) = 0;

    virtual bool StepExecution() = 0;

    void Reset()
    {
        m_rSync.wantsReset = true;
        // If we were waiting for input then we need to update the condition variable to re-enable execution
        if (m_waitingForCharInput || m_waitingForValInput)
        {
            m_waitingForCharInput = m_waitingForValInput = false;
            m_rSync.waitingOnInputCV.notify_one();
        }
    }

    void ResetOutput()
    {
        m_rOutputStream.str(std::string());
        m_rExecutionHistoryStream.str(std::string());
    }

    virtual void ResetImplementation() = 0;

    void InputChar(int val)
    {
        OnInput(val);
        m_waitingForCharInput = false;
    }

    void InputVal(int val)
    {
        OnInput(val);
        m_waitingForValInput = false;
    }

    bool IsRunning() const
    {
        return m_bIsRunning;
    }

    bool IsWaitingForValInput() const
    {
        return m_waitingForValInput;
    }

    bool IsWaitingForCharInput() const
    {
        return m_waitingForCharInput;
    }

    virtual void RenderWindows() = 0;
    virtual void CacheState() = 0;

  protected:
    virtual void OnInput(int val) = 0;

    RuntimeSyncronisationStruct& m_rSync;

    std::ostringstream& m_rOutputStream;
    std::ostringstream& m_rExecutionHistoryStream;

    bool m_waitingForCharInput = false;
    bool m_waitingForValInput = false;
    bool m_bIsRunning = false;
};
