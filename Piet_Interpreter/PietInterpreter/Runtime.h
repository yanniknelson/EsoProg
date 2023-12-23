#include "PietTextTokeniser.h"
#include "PietImageTokeniser.h"
#include "PietStack.h"

#include <iostream>
#include <sstream>

#include <string>

class Runtime
{

	PietTextTokeniser m_textTokeniser;
	PietImageTokeniser m_ImageTokeniser;

	bool m_bFinished = false;

	PietStack m_stack;
	std::string m_codeStr = "";
	std::stringstream m_code;

	std::string& m_rOutputStr;

	void StepExecution(PietToken& token, PietToken& value);

	void ResetStream()
	{
		m_code = std::stringstream(m_codeStr);
		m_textTokeniser.SetTextStream(m_code);
		m_rOutputStr = "";
		m_bFinished = false;
	}

public:

	static const PietToken m_tDefaultToken;

	bool IsFinished() { return m_bFinished; }

	Runtime(std::string& outputString) : m_rOutputStr(outputString) {}

	void StepExecution();

	void SetTextStream(std::string str)
	{
		m_codeStr = str;
		m_code.swap(std::stringstream(m_codeStr.c_str()));
		m_textTokeniser.SetTextStream(m_code);
		m_rOutputStr = "";
		m_bFinished = false;
	}

	void Reset()
	{
		ResetStream();
		m_stack.Clear();
	}

	int Run();

	const std::deque<int>& GetStack() { return m_stack.GetStack(); }
};