#pragma once

#include "ITokeniser.h"
#include "IMemoryArray.h"
#include "BrainFckToken.h"
#include "BrainFckTokeniser.h"
#include "BrainFckParser.h"

#include "ELanguages.h"

#include <iostream>
#include <sstream>

#include <CRuntime.h>
#include <string>
#include <vector>

class BrainFckRuntime : public CRuntime<BrainFckToken>
{
public:
	BrainFckRuntime(RuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream)
		: CRuntime(rSync, rOutputStream, rExecutionhistoryStream)
		, m_parser(&m_tokeniser)
	{
		m_tokeniser.SetTextStream(m_code);
	};

	virtual ELanguages::Enum GetRuntimeLanguage() const override { return ELanguages::Brainfck; }
	virtual std::vector<std::string> GetSupportedFileTypes() const override { return { ".txt" }; }

	void Reset()
	{
		ResetTokenisers();
		m_array.Clear();
	}

	virtual void RenderWindows(RuntimeSyncronisationStruct& rSync) override;
	virtual void CacheState() override;

private:

	BrainFckTokeniser m_tokeniser;
	BrainFckParser m_parser;
	std::shared_ptr<BrainFckProgram> m_pProgramAST{ nullptr };

	int m_currentIndex{ 0 };
	IMemoryArray<uint8_t> m_array;
	int m_cachedIndex{ 0 };
	IMemoryArray<uint8_t> m_cachedArray;

	virtual void OnSourceSet() override;

	virtual void OnInput(int val) override;

	virtual BrainFckToken StepExecution_Internal() override;
	virtual void ResetTokenisers() override
	{
		m_array.Clear();
		m_tokeniser.SetTextStream(m_code);
	}
};