#include "PietTextTokeniser.h"
#include "PietImageTokeniser.h"
#include "PietStack.h"

#include <iostream>
#include <sstream>

#include <string>

class Runtime
{
public:

	enum class SourceType
	{
		Text,
		Image
	};

	static const PietToken m_tDefaultToken;

	bool IsFinished() { return m_bFinished; }

	Runtime(std::string& outputString) : m_rOutputStr(outputString) {}

	void StepExecution(SourceType sourceType);

	void SetTextStream(std::string str)
	{
		m_codeStr = str;
		m_code.swap(std::stringstream(m_codeStr.c_str()));
		m_textTokeniser.SetTextStream(m_code);
		m_rOutputStr = "";
		m_bFinished = false;
	}

	void SetImage(const unsigned char* imageData, const int imageWidth, const int imageHeight)
	{
		m_imageTokeniser.SetImage(imageData, imageWidth, imageHeight);
	}

	void SetCodelSize(const int size);

	void Reset()
	{
		ResetTokenisers();
		m_stack.Clear();
	}

	int RunFromStart(SourceType sourceType);
	int Run();
	void InputChar(int val);
	void InputVal(int val);

	bool IsWaitingForValInput() const;
	bool IsWaitingForCharInput() const;

	const std::deque<int>& GetStack() { return m_stack.GetStack(); }

private:
	PietTextTokeniser m_textTokeniser;
	PietImageTokeniser m_imageTokeniser;

	bool m_bFinished = false;

	PietStack m_stack;
	std::string m_codeStr = "";
	std::stringstream m_code;
	bool m_waitingForCharInput = false;
	bool m_waitingForValInput = false;

	SourceType m_currentSourceType{ SourceType::Text };

	std::string& m_rOutputStr;

	void StepExecution(PietToken& token);

	void ResetTokenisers()
	{
		m_code = std::stringstream(m_codeStr);
		m_textTokeniser.SetTextStream(m_code);
		m_imageTokeniser.Reset();
		m_rOutputStr = "";
		m_bFinished = false;
	}
};