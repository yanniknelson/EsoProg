#pragma once

#include "IRuntime.h"

#include <ITokeniser.h>
#include <ELanguages.h>

#include <iostream>
#include <sstream>

#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>

template<typename TokenClass>
class CRuntime : public IRuntime
{
public:
	CRuntime(std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : IRuntime(rOutputStream, rExecutionhistoryStream) {}

	virtual ELanguages::Enum GetRuntimeLanguage() const = 0;

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

	virtual void RenderWindows(RuntimeSyncronisationStruct& rSync) = 0;
	virtual void CacheState() = 0;

protected:
	std::string m_codeStr = "";
	std::stringstream m_code;

	ITokeniser<TokenClass>* m_activeTokeniser = nullptr;

	virtual void OnInput(int val) = 0;

	virtual TokenClass StepExecution_Internal() = 0;
	virtual void ResetTokenisers() = 0;
};