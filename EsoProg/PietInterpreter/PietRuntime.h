#include "PietTextTokeniser.h"
#include "PietImageTokeniser.h"
#include "PietStack.h"

#include <iostream>
#include <sstream>

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Runtime
{
public:

	enum class SourceType
	{
		Text,
		Image,
		Invalid
	};

	static const PietToken m_tDefaultToken;

	Runtime(std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream) : m_rOutputStream(rOutputStream), m_rExecutionHistoryStream(rExecutionhistoryStream) {}

	void StepExecution();

	void SetTextStream(std::string str)
	{
		m_codeStr = str;
		m_code.swap(std::stringstream(m_codeStr.c_str()));
		m_textTokeniser.SetTextStream(m_code);
		m_rOutputStream.str(std::string());
		m_rExecutionHistoryStream.str(std::string());
		if (!str.empty())
		{
			m_currentSourceType = SourceType::Text;
			m_activeTokeniser = (m_currentSourceType == SourceType::Text) ? (PietTokeniser*)&m_textTokeniser : (PietTokeniser*)&m_imageTokeniser;
		}
		ResetTokenisers();
	}

	void SetImage(GLuint* pTexture, const unsigned char* imageData, const int imageWidth, const int imageHeight)
	{
		m_pTexture = pTexture;
		m_aspectRatio = (float)imageHeight / (float)imageWidth;
		m_imageTokeniser.SetImage(imageData, imageWidth, imageHeight);
		m_currentSourceType = SourceType::Image;
		m_activeTokeniser = (m_currentSourceType == SourceType::Text) ? (PietTokeniser*)&m_textTokeniser : (PietTokeniser*)&m_imageTokeniser;
	}
	
	void UnsetImage()
	{
		m_currentSourceType = SourceType::Invalid;
		m_activeTokeniser = (m_currentSourceType == SourceType::Image) ? (PietTokeniser*)&m_imageTokeniser : (PietTokeniser*)&m_textTokeniser;
		m_pTexture = nullptr;
		m_aspectRatio = 1.f;
		m_imageTokeniser.UnsetImage();
	}

	void SetCodelSize(const int size);

	void Reset()
	{
		ResetTokenisers();
		m_stack.Clear();
	}

	void RunFromStart();
	void Run();
	void InputChar(int val);
	void InputVal(int val);

	bool IsRunning() const;
	bool IsWaitingForValInput() const;
	bool IsWaitingForCharInput() const;

	const std::deque<int>& GetStack() { return m_stack.GetStack(); }

	void RenderWindows();

private:

	PietTextTokeniser m_textTokeniser;
	PietImageTokeniser m_imageTokeniser;

	PietStack m_stack;
	std::string m_codeStr = "";
	std::stringstream m_code;
	bool m_waitingForCharInput = false;
	bool m_waitingForValInput = false;
	bool m_bIsRunning = false;

	GLuint* m_pTexture = nullptr;
	float m_aspectRatio = 1.f;
	std::string m_codelSizeStr{ "1" };
	int m_codelSize{ 1 };

	SourceType m_currentSourceType{ SourceType::Text };
	PietTokeniser* m_activeTokeniser = nullptr;

	std::ostringstream& m_rOutputStream;
	std::ostringstream& m_rExecutionHistoryStream;

	void StepExecution_Internal();

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