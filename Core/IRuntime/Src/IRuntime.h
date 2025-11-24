#pragma once

#include <ELanguages.h>

#include <iostream>
#include <sstream>

#include <string>
#include <mutex>
#include <atomic>
#include <condition_variable>

struct RuntimeSyncronisationStruct
{
	std::atomic<int> iterations = 0;
	std::atomic<int> instructionWaitTime = 0;
	std::atomic<bool> exit = false;
	std::atomic<bool> renderWantsState = false;
	std::condition_variable finishedStateWithCv;
	std::mutex finishedWithStateMtx;
	std::mutex runtimeStateMtx;

	std::condition_variable waitingOnInputCV;
	std::mutex waitingOnInputMtx;
};

class IRuntime
{
public:

	IRuntime(std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : m_rOutputStream(rOutputStream), m_rExecutionHistoryStream(rExecutionhistoryStream) {}

	virtual ELanguages::Enum GetRuntimeLanguage() const = 0;

	virtual void SetSourceCode(std::string str) = 0;

	virtual bool StepExecution() = 0;

	void Reset()
	{
		m_rOutputStream.str(std::string());
		m_rExecutionHistoryStream.str(std::string());
		ResetTokenisers();
	}

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

	virtual void RenderWindows(RuntimeSyncronisationStruct& rSync) = 0;
	virtual void CacheState() = 0;

protected:
	virtual void OnInput(int val) = 0;
	virtual void ResetTokenisers() = 0;

	std::ostringstream& m_rOutputStream;
	std::ostringstream& m_rExecutionHistoryStream;

	bool m_waitingForCharInput = false;
	bool m_waitingForValInput = false;
	bool m_bIsRunning = false;
};