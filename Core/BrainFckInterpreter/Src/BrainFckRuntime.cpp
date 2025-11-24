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

PietToken PietRuntime::StepExecution_Internal()
{
	const PietToken token = m_activeTokeniser->Pop();

	int top = 0;
	int second = 0;
	int val = 0;

	switch (token.m_type)
	{
	case(PietToken::TokenType::Push):
	{
		m_stack.Push(token.m_value);
		break;
	}
	case(PietToken::TokenType::Pop):
	{
		if (m_stack.GetSize() > 0)
		{
			m_stack.Pop();
		}
		break;
	}
	case(PietToken::TokenType::Add):
	{
		if (m_stack.GetSize() >= 2)
		{
			m_stack.Push(m_stack.Pop() + m_stack.Pop());
		}
		break;
	}
	case(PietToken::TokenType::Subtract):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			m_stack.Push(second - top);
		}
		break;
	}
	case(PietToken::TokenType::Multiply):
	{
		if (m_stack.GetSize() >= 2)
		{
			m_stack.Push(m_stack.Pop() * m_stack.Pop());
		}
		break;
	}
	case(PietToken::TokenType::Divide):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			m_stack.Push(second / top);
		}
		break;
	}
	case(PietToken::TokenType::Modulo):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			m_stack.Push((top + (second % top)) % top);
		}
		break;
	}
	case(PietToken::TokenType::Not):
	{
		if (m_stack.GetSize() > 0)
		{
			val = m_stack.Pop() == 0 ? 1 : 0;
			m_stack.Push(val);
		}
		break;
	}
	case(PietToken::TokenType::Greater):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			val = (second > top) ? 1 : 0;
			m_stack.Push(val);
		}
		break;
	}
	case(PietToken::TokenType::Pointer):
	{
		if (!m_stack.Empty())
		{
			top = m_stack.Pop();
			m_imageTokeniser.RotateDirectionPointer(top);
		}
		break;
	}
	case(PietToken::TokenType::Switch):
	{
		if (!m_stack.Empty())
		{
			top = m_stack.Pop();
			if (top % 2 != 0)
			{
				m_imageTokeniser.ToggleCodelChooser();
			}
		}
		break;
	}
	case(PietToken::TokenType::Duplicate):
	{
		if (m_stack.GetSize() > 0)
		{
			val = m_stack.Pop();
			m_stack.Push(val);
			m_stack.Push(val);
		}
		break;
	}
	case(PietToken::TokenType::Roll):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			m_stack.Roll(second, top);
		}
		break;
	}
	case(PietToken::TokenType::Input_Char):
		m_waitingForCharInput = true;
		break;
	case(PietToken::TokenType::Input_Val):
		m_waitingForValInput = true;
		break;
	case(PietToken::TokenType::Output_Char):
	{
		if (m_stack.GetSize() > 0)
		{
			m_rOutputStream << (char)m_stack.Pop();
		}
		break;
	}
	case(PietToken::TokenType::Output_Val):
	{
		if (m_stack.GetSize() > 0)
		{
			m_rOutputStream << std::to_string(m_stack.Pop());
		}
		break;
	}
	case(PietToken::TokenType::NOP):
	{
		return PietToken::TokenType::NOP;
	}
	case (PietToken::TokenType::End):
	{
		m_bIsRunning = false;
		break;
	}
	default:
		break;
	}

	return token;
}

void PietRuntime::OnSourceSet()
{
	if (!m_codeStr.empty())
	{
		m_currentSourceType = SourceType::Text;
		m_activeTokeniser = (TPietTokeniser*)&m_textTokeniser;
	}
	else
	{
		m_currentSourceType = SourceType::Image;
		m_activeTokeniser = (TPietTokeniser*)&m_imageTokeniser;
	}
}

void PietRuntime::OnInput(int val)
{
	m_stack.Push(val);
}

void PietRuntime::RenderImageDisplay(RuntimeSyncronisationStruct& rSync)
{
	if (ImGui::Begin("Piet Image"))
	{
		if (m_currentSourceType == SourceType::Image)
		{
			const ImVec2 area = ImGui::GetContentRegionAvail();
			ImVec2 desired = ImVec2(area.x, (int)area.x * m_aspectRatio);
			if (desired.y > area.y)
			{
				desired = ImVec2((int)area.y * (1 / m_aspectRatio), area.y);
			}
			ImGui::Image((ImTextureID)(intptr_t)(*m_pTexture), desired);
		}

		if (ImGui::Button("Run"))
		{
			rSync.iterations = -1;
		}

		ImGui::SameLine();
		{
			int currentinstructionWaitTime = rSync.instructionWaitTime.load();
			int newInstructionWaitTime = currentinstructionWaitTime;
			ImGui::SliderInt("Execution Speed", &newInstructionWaitTime, 0, 1000);
			rSync.instructionWaitTime.compare_exchange_strong(currentinstructionWaitTime, newInstructionWaitTime);
		}

		if (rSync.iterations == -1)
		{
			ImGui::SameLine();
			if (ImGui::Button("Pause"))
			{
				rSync.iterations = 0;
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Step"))
		{
			++rSync.iterations;
		}

		ImGui::SameLine();
		{
			int newCodelSize = m_codelSize;
			ImGui::InputText("##codelSize", &m_codelSizeStr, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CallbackEdit, ValueInputChanged, &newCodelSize);
			if (newCodelSize != m_codelSize)
			{
				m_codelSize = newCodelSize;
				SetCodelSize(m_codelSize);
			}
		}

		ImGui::Text("Current Block Start Location: %s - End Location: %s",
			m_imageTokeniser.GetCachedBlockStartLocation().toString().c_str(),
			m_imageTokeniser.GetCachedBlockEndLocation().toString().c_str());
		ImGui::Text("Current Block Start Dir: %s - End Dir: %s",
			PietImageTokeniser::i_directionIcons[static_cast<int>(m_imageTokeniser.GetCachedBlockStartDirectionPointer())],
			PietImageTokeniser::i_directionIcons[static_cast<int>(m_imageTokeniser.GetCachedBlockEndDirectionPointer())]);
		ImGui::Text("Current Block Start CC: %s - End CC: %s",
			PietImageTokeniser::i_directionIcons[static_cast<int>(m_imageTokeniser.GetCachedBlockStartCodelChoser())],
			PietImageTokeniser::i_directionIcons[static_cast<int>(m_imageTokeniser.GetCachedBlockEndCodelChoser())]);

		ImGui::End();
	}
}

void PietRuntime::RenderWindows(RuntimeSyncronisationStruct& rSync)
{
	m_cachedStack.DisplayStack();
	RenderImageDisplay(rSync);
}

void PietRuntime::CacheState()
{
	m_cachedStack = m_stack;
	m_imageTokeniser.CopyState();
}

void PietRuntime::SetCodelSize(const int size)
{
	m_imageTokeniser.SetCodelSize(size);
}