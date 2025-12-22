#pragma once

#include <ELanguages.h>        // for ELangauges::Enum

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <sstream>             // for ostringstream
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////
struct SRuntimeSyncronisationStruct
{
    std::atomic<int> m_iterations = 0;
    std::atomic<int> m_instructionWaitTime = 0;
    std::atomic<bool> m_bExit = false;
    std::atomic<bool> m_bRenderWantsState = false;
    std::atomic<bool> m_bWantsReset = false;
    std::atomic<bool> m_bResetCodeSource = false;
    std::condition_variable m_finishedStateWithCv;
    std::mutex m_finishedWithStateMtx;
    std::mutex m_runtimeStateMtx;

    std::condition_variable m_waitingOnInputCV;
    std::mutex m_waitingOnInputMtx;
};

//////////////////////////////////////////////////////////////
class IRuntime
{
  public:
    //////////////////////////////////////////////////////////////
    IRuntime(SRuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : m_rSync(rSync), m_rOutputStream(rOutputStream), m_rExecutionHistoryStream(rExecutionhistoryStream)
    {
    }

    virtual ELanguages::Enum GetRuntimeLanguage() const = 0;
    virtual std::vector<std::string> GetSupportedFileTypes() const = 0;

    virtual void SetSourceCode(std::string str) = 0;

    virtual bool StepExecution() = 0;

    //////////////////////////////////////////////////////////////
    void RequestReset()
    {
        m_rSync.m_bWantsReset = true;
        // If we were waiting for input then we need to update the condition variable to re-enable execution
        if (m_waitingForCharInput || m_waitingForValInput)
        {
            m_waitingForCharInput = m_waitingForValInput = false;
            m_rSync.m_waitingOnInputCV.notify_one();
        }
    }

    //////////////////////////////////////////////////////////////
    void Reset()
    {
        ResetCodeStream();
        ResetImplementation();
        ResetOutput();
    }

    //////////////////////////////////////////////////////////////
    void InputChar(int val)
    {
        OnInput(val);
        m_waitingForCharInput = false;
    }

    //////////////////////////////////////////////////////////////
    void InputVal(int val)
    {
        OnInput(val);
        m_waitingForValInput = false;
    }

    //////////////////////////////////////////////////////////////
    bool IsRunning() const
    {
        return m_bIsRunning;
    }

    //////////////////////////////////////////////////////////////
    bool IsWaitingForValInput() const
    {
        return m_waitingForValInput;
    }

    //////////////////////////////////////////////////////////////
    bool IsWaitingForCharInput() const
    {
        return m_waitingForCharInput;
    }

    virtual void RenderWindows() = 0;
    virtual void CacheState() = 0;

  protected:
    virtual void OnInput(int val) = 0;

    //////////////////////////////////////////////////////////////
    void ResetOutput()
    {
        m_rOutputStream.str(std::string());
        m_rExecutionHistoryStream.str(std::string());
    }

    virtual void ResetCodeStream() = 0;
    virtual void ResetImplementation() = 0;

    SRuntimeSyncronisationStruct& m_rSync;

    std::ostringstream& m_rOutputStream;
    std::ostringstream& m_rExecutionHistoryStream;

    bool m_waitingForCharInput = false;
    bool m_waitingForValInput = false;
    bool m_bIsRunning = false;
};
