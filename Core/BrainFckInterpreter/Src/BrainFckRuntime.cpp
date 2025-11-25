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
	const BrainFckToken token = m_activeTokeniser->Pop();

	switch (token.m_type)
	{
	case(BrainFckToken::TokenType::Move_Right):
	{
		
		break;
	}
	case(BrainFckToken::TokenType::Move_Left):
	{
		
		break;
	}
	case(BrainFckToken::TokenType::Increment):
	{
		
		break;
	}
	case(BrainFckToken::TokenType::Decrement):
	{
		
		break;
	}
	case(BrainFckToken::TokenType::Output_Char):
	{
		if (m_stack.GetSize() > 0)
		{
			m_rOutputStream << (char)m_stack.Pop();
		}
		break;
	}
	case(BrainFckToken::TokenType::Input_Char):
	{
		m_waitingForCharInput = true;
		break;
	}
	case(BrainFckToken::TokenType::Branch_Start):
	{
		break;
	}
	case(BrainFckToken::TokenType::Branch_End):
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

void BrainFckRuntime::OnInput(int val)
{
	m_stack.Push(val);
}

void BrainFckRuntime::RenderWindows(RuntimeSyncronisationStruct& rSync)
{
	m_cachedStack.DisplayStack();
}

void BrainFckRuntime::CacheState()
{
	m_cachedStack = m_stack;
}