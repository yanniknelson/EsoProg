#pragma once

#include <ITokeniser.h>
#include <IRuntime.h>
#include <PietRuntime.h>
#include <SmartEnum.h>

#include <iostream>
#include <sstream>

#define ELANGUAGES(x)\
    x(Piet)

CreateSmartEnum(ELanguages, ELANGUAGES);

#undef ELANGUAGES

class RuntimeWrapper
{
public:
	ELanguages::Enum GetRuntimeLanguage() const
	{
		return ELanguages::Piet;
	}

	void* GetRuntimePtr()
	{
		return &m_selectedRuntime;
	}

	RuntimeWrapper(std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : m_selectedRuntime(rOutputStream, rExecutionhistoryStream) {}

	void StepExecution()
	{
		m_selectedRuntime.StepExecution();
	}

	void Reset()
	{
		m_selectedRuntime.Reset();
	}

	void RunFromStart()
	{
		m_selectedRuntime.RunFromStart();
	}

	void Run()
	{
		m_selectedRuntime.Run();
	}

	void InputChar(int val)
	{
		m_selectedRuntime.InputChar(val);
	}

	void InputVal(int val)
	{
		m_selectedRuntime.InputVal(val);
	}

	bool IsRunning() const
	{
		m_selectedRuntime.IsRunning();
	}

	bool IsWaitingForValInput() const
	{
		return m_selectedRuntime.IsWaitingForValInput();
	}

	bool IsWaitingForCharInput() const
	{
		return m_selectedRuntime.IsWaitingForCharInput();
	}

	void RenderWindows(RuntimeSyncronisationStruct& rSync)
	{
		m_selectedRuntime.RenderWindows(rSync);
	}

	void CacheState()
	{
		m_selectedRuntime.CacheState();
	}

protected:
	PietRuntime m_selectedRuntime;
};