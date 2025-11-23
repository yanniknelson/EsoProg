#pragma once

#include <ITokeniser.h>

#include <iostream>
#include <sstream>

#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
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

template<typename TokenClass>
class IRuntime
{
public:
	IRuntime(std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : m_rOutputStream(rOutputStream), m_rExecutionHistoryStream(rExecutionhistoryStream) {}

	void StepExecution()
	{
		if (m_waitingForCharInput || m_waitingForValInput)
		{
			return;
		}

		const TokenClass token = StepExecution_Internal();
		if (token.m_type != TokenClass::TokenType::NOP)
		{
			m_rExecutionHistoryStream << token << std::endl;
		}
	}

	void Reset()
	{
		ResetTokenisers();
	}

	void RunFromStart()
	{
		ResetTokenisers();
		m_bIsRunning = true;
		Run();
	}

	void Run()
	{
		while (m_bIsRunning)
		{
			StepExecution();
		}
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
	std::string m_codeStr = "";
	std::stringstream m_code;
	bool m_waitingForCharInput = false;
	bool m_waitingForValInput = false;
	bool m_bIsRunning = false;

	ITokeniser<TokenClass>* m_activeTokeniser = nullptr;

	std::ostringstream& m_rOutputStream;
	std::ostringstream& m_rExecutionHistoryStream;

	virtual void OnInput(int val) = 0;

	virtual TokenClass StepExecution_Internal() = 0;
	virtual void ResetTokenisers() = 0;

	void ResetRuntime()
	{
		m_code = std::stringstream(m_codeStr);
		ResetTokenisers();
		m_rOutputStream.str(std::string());
		m_rExecutionHistoryStream.str(std::string());
		m_bIsRunning = false;
		m_waitingForCharInput = false;
		m_waitingForValInput = false;
	}
};