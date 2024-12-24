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

	Runtime(std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : m_rOutputStream(rOutputStream), m_rExecutionHistoryStream(rExecutionhistoryStream) {}

	void StepExecution();
	void StepExecution(SourceType sourceType);

	void SetTextStream(std::string str)
	{
		m_codeStr = str;
		m_code.swap(std::stringstream(m_codeStr.c_str()));
		m_textTokeniser.SetTextStream(m_code);
		m_rOutputStream.str(std::string());
		m_rExecutionHistoryStream.str(std::string());
	}

	void SetImage(const unsigned char* imageData, const int imageWidth, const int imageHeight)
	{
		m_imageTokeniser.SetImage(imageData, imageWidth, imageHeight);
	}
	
	void UnsetImage()
	{
		m_imageTokeniser.UnsetImage();
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

	bool IsRunning() const;
	bool IsWaitingForValInput() const;
	bool IsWaitingForCharInput() const;

	PietImageTokeniser::Location GetCurrentBlockStartLocation() const;
	PietImageTokeniser::Location GetCurrentBlockEndLocation() const;
	PietImageTokeniser::Direction GetCurrentBlockStartDirectionPointer() const;
	PietImageTokeniser::Direction GetCurrentBlockEndDirectionPointer() const;
	PietImageTokeniser::Direction GetCurrentBlockStartCodelChoser() const;
	PietImageTokeniser::Direction GetCurrentBlockEndCodelChoser() const;

	const std::deque<int>& GetStack() { return m_stack.GetStack(); }

private:
	PietTextTokeniser m_textTokeniser;
	PietImageTokeniser m_imageTokeniser;

	PietStack m_stack;
	std::string m_codeStr = "";
	std::stringstream m_code;
	bool m_waitingForCharInput = false;
	bool m_waitingForValInput = false;
	bool m_bIsRunning = false;

	SourceType m_currentSourceType{ SourceType::Text };

	std::ostringstream& m_rOutputStream;
	std::ostringstream& m_rExecutionHistoryStream;

	void StepExecution(PietToken& token);

	void ResetTokenisers()
	{
		m_code = std::stringstream(m_codeStr);
		m_textTokeniser.SetTextStream(m_code);
		m_imageTokeniser.Reset();
		m_rOutputStream.str(std::string());
		m_rExecutionHistoryStream.str(std::string());
		m_bIsRunning = false;
		m_waitingForCharInput = false;
		m_waitingForValInput = false;
	}
};