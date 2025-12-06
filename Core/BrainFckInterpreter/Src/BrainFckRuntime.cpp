#pragma once

#include "BrainFckRuntime.h"
#include "BrainFckToken.h"

//ImGui imports
#include <imgui.h>
#include <imgui_stdlib.h>

//OpenGL imports
#include "GLFW/glfw3.h"

//Other imports
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <ImGuiValueChangeCallbacks.h>

BrainFckToken BrainFckRuntime::StepExecution_Internal()
{
	const BrainFckToken token = m_tokeniser.Pop();

	switch (token.m_type)
	{
	case(BrainFckToken::TokenType::Move_Right):
	{
		++m_currentIndex;
		break;
	}
	case(BrainFckToken::TokenType::Move_Left):
	{
		--m_currentIndex;
		break;
	}
	case(BrainFckToken::TokenType::Increment):
	{
		m_array.Increment(m_currentIndex);
		break;
	}
	case(BrainFckToken::TokenType::Decrement):
	{
		m_array.Decrement(m_currentIndex);
		break;
	}
	case(BrainFckToken::TokenType::Output_Char):
	{
		m_rOutputStream << (char)m_array.Get(m_currentIndex);
		break;
	}
	case(BrainFckToken::TokenType::Input_Char):
	{
		m_waitingForCharInput = true;
		break;
	}
	case(BrainFckToken::TokenType::Loop_Start):
	{
		break;
	}
	case(BrainFckToken::TokenType::Loop_End):
	{
		break;
	}
	case(BrainFckToken::TokenType::NOP):
	{
		return BrainFckToken::TokenType::NOP;
	}
	case (BrainFckToken::TokenType::End):
	{
		m_bIsRunning = false;
		break;
	}
	default:
		break;
	}

	return token;
}

void BrainFckRuntime::OnSourceSet()
{
	Program* pProgram = static_cast<Program*>(m_parser.Parse());
	delete pProgram;
}

void BrainFckRuntime::OnInput(int val)
{
	m_array.Set(m_currentIndex, val);
}

void BrainFckRuntime::RenderWindows()
{
	m_cachedArray.DisplayArray(m_cachedIndex);
}

void BrainFckRuntime::CacheState()
{
	m_cachedIndex = m_currentIndex;
	m_cachedArray = m_array;
}