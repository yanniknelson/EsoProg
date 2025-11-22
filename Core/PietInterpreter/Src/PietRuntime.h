#pragma once

#include "ITokeniser.h"
#include "PietTextTokeniser.h"
#include "PietImageTokeniser.h"
#include "PietStack.h"

#include <iostream>
#include <sstream>

#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>

#include "GLFW/glfw3.h"

class Runtime
{
	using TPietTokeniser = ITokeniser<PietToken>;
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
		std::stringstream tmp(m_codeStr.c_str());
		m_code.swap(tmp);
		m_textTokeniser.SetTextStream(m_code);
		m_rOutputStream.str(std::string());
		m_rExecutionHistoryStream.str(std::string());
		if (!str.empty())
		{
			m_currentSourceType = SourceType::Text;
			m_activeTokeniser = (m_currentSourceType == SourceType::Text) ? (TPietTokeniser*)&m_textTokeniser : (TPietTokeniser*)&m_imageTokeniser;
		}
		ResetTokenisers();
	}

	void SetImage(GLuint* pTexture, const unsigned char* imageData, const int imageWidth, const int imageHeight)
	{
		m_pTexture = pTexture;
		m_aspectRatio = (float)imageHeight / (float)imageWidth;
		m_imageTokeniser.SetImage(imageData, imageWidth, imageHeight);
		m_currentSourceType = SourceType::Image;
		m_activeTokeniser = (m_currentSourceType == SourceType::Text) ? (TPietTokeniser*)&m_textTokeniser : (TPietTokeniser*)&m_imageTokeniser;
	}
	
	void UnsetImage()
	{
		m_currentSourceType = SourceType::Invalid;
		m_activeTokeniser = (m_currentSourceType == SourceType::Image) ? (TPietTokeniser*)&m_imageTokeniser : (TPietTokeniser*)&m_textTokeniser;
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

	const std::deque<int>& GetStack() { return m_cachedStack.GetStack(); }

	struct SyncronisationStruct
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

		std::string m_instructionWaitTimeStr{ "0" };
	};

	void RenderWindows(SyncronisationStruct& rSync);
	void CopyState();

private:

	PietTextTokeniser m_textTokeniser;
	PietImageTokeniser m_imageTokeniser;

	PietStack m_stack;
	PietStack m_cachedStack;
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
	TPietTokeniser* m_activeTokeniser = nullptr;

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