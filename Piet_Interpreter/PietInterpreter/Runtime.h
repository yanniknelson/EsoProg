#include "PietTextTokeniser.h"
#include "PietStack.h"
#include <string>

class Runtime
{

	enum class Direction
	{
		Up, Right, Down, Left
	};

	struct Location
	{
		int x;
		int y;
	};

	PietTextTokeniser m_textTokeniser;
	Direction dp;
	Direction cc;
	Location m_currentPixel{ 0, 0 };

	bool m_bFinished = false;

	PietToken m_tStartToken{ PietToken::TokenType::Start };

	PietStack m_stack;
	std::string m_codeStr = "";
	std::istrstream m_code = std::istrstream("");

	std::string& m_rOutputStr;

	void StepExecution(PietToken& token, PietToken& value);

	void ResetStream()
	{
		m_code = std::istrstream(m_codeStr.c_str());
		m_textTokeniser.SetStream(m_code);
		m_rOutputStr = "";
		m_bFinished = false;
	}

public:

	bool IsFinished() { return m_bFinished; }

	Runtime(std::string& outputString) : m_rOutputStr(outputString) {}

	void StepExecution();

	void SetStream(std::string str)
	{
		m_codeStr = str;
		m_code = std::istrstream(m_codeStr.c_str());
		m_textTokeniser.SetStream(m_code);
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