#pragma once

#include "ITokeniser.h"
#include "BrainFckToken.h"
#include "BrainFckTokeniser.h"

#include "ELanguages.h"

#include <iostream>
#include <sstream>

#include <Stack.h>
#include <CRuntime.h>
#include <string>
#include <vector>

class BrainFckRuntime : public CRuntime<BrainFckToken>
{
public:
	BrainFckRuntime(std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : CRuntime(rOutputStream, rExecutionhistoryStream) {};

	virtual ELanguages::Enum GetRuntimeLanguage() const override { return ELanguages::Brainfck; }
	virtual std::vector<std::string> GetSupportedFileTypes() const override { return { ".txt" }; }

	void Reset()
	{
		ResetTokenisers();
		m_stack.Clear();
	}

	virtual void RenderWindows(RuntimeSyncronisationStruct& rSync) override;
	virtual void CacheState() override;

private:

	BrainFckTokeniser m_tokeniser;

	Stack m_stack;
	Stack m_cachedStack;

	float m_aspectRatio = 1.f;
	std::string m_codelSizeStr{ "1" };
	int m_codelSize{ 1 };

	virtual void OnSourceSet() override {};

	virtual void OnInput(int val) override;

	virtual BrainFckToken StepExecution_Internal() override;
	virtual void ResetTokenisers() override
	{
		m_stack.Clear();
		m_tokeniser.SetTextStream(m_code);
	}
};