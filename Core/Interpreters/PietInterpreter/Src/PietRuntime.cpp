#pragma once

#include "PietRuntime.h"

#include "PietImageTokeniser.h"         // for PietImageTokeniser
#include "PietToken.h"                  // for PietToken

#include <ImGuiValueChangeCallbacks.h>  // for ValueInputChanged
#include <CRuntime.h>                   // for CRuntime
#include <IRuntime.h>                   // for SRuntimeSyncronisationStruct
#include <ELanguages.h>                 // for ELanguages::Enum

#include <imgui.h>                      // for ImGui structs and functions
#include <imgui_stdlib.h>               // for InputText and others with std::string input
#include <GLFW/glfw3.h>                 // for GLuint
#include <gl/GL.h>                      // for GLuint

#include <string>                       // std::to_string
#include <sstream>                      // std::ostringstream
#include <vector>                       // std::ostringstream

//////////////////////////////////////////////////////////////
PietRuntime::PietRuntime(SRuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream)
    : CRuntime(rSync, rOutputStream, rExecutionhistoryStream)
{
    m_activeTokeniser = (TPietTokeniser*)&m_textTokeniser;
};

//////////////////////////////////////////////////////////////
ELanguages::Enum PietRuntime::GetRuntimeLanguage() const
{
    return ELanguages::Piet;
}

//////////////////////////////////////////////////////////////
std::vector<std::string> PietRuntime::GetSupportedFileTypes() const
{
    return { ".txt", ".jpg", ".png", ".gif", ".ppm" };
}

//////////////////////////////////////////////////////////////
void PietRuntime::ResetImplementation()
{
    m_stack.Clear();
    m_textTokeniser.SetTextStream(m_code);
    m_imageTokeniser.Reset();
};

//////////////////////////////////////////////////////////////
void PietRuntime::RenderWindows()
{
    m_cachedStack.DisplayStack();
    RenderImageDisplay();
}

//////////////////////////////////////////////////////////////
void PietRuntime::CacheState()
{
    m_cachedStack = m_stack;
    m_imageTokeniser.CopyState();
}

//////////////////////////////////////////////////////////////
void PietRuntime::SetImage(GLuint* pTexture, const unsigned char* imageData, const int imageWidth, const int imageHeight)
{
    m_currentSourceType = ESourceType::Image;
    m_pTexture = pTexture;
    m_aspectRatio = (float)imageHeight / (float)imageWidth;
    m_imageTokeniser.SetImage(imageData, imageWidth, imageHeight);
    m_codelSize = m_imageTokeniser.GetCodelSize();
    m_codelSizeStr = std::to_string(m_codelSize);
    m_activeTokeniser = &m_imageTokeniser;
}

//////////////////////////////////////////////////////////////
void PietRuntime::UnsetImage()
{
    m_currentSourceType = ESourceType::Invalid;
    m_activeTokeniser = (TPietTokeniser*)&m_textTokeniser;
    m_pTexture = nullptr;
    m_aspectRatio = 1.f;
    m_imageTokeniser.UnsetImage();
}

//////////////////////////////////////////////////////////////
void PietRuntime::SetCodelSize(const int size)
{
    m_imageTokeniser.SetCodelSize(size);
}

//////////////////////////////////////////////////////////////
void PietRuntime::OnInput(int val)
{
    m_stack.Push(val);
}

//////////////////////////////////////////////////////////////
void PietRuntime::OnSourceSet()
{
    if (m_bForceImage)
    {
        m_currentSourceType = ESourceType::Image;
        m_activeTokeniser = (TPietTokeniser*)&m_imageTokeniser;
        m_bForceImage = false;
    }
    else
    {
        m_currentSourceType = ESourceType::Text;
        m_activeTokeniser = (TPietTokeniser*)&m_textTokeniser;
    }
}

//////////////////////////////////////////////////////////////
bool PietRuntime::ShouldEnd(const PietToken& token)
{
    return token.m_type == PietToken::ETokenType::End;
}

//////////////////////////////////////////////////////////////
PietToken PietRuntime::StepExecution_Internal()
{
    const PietToken token = m_activeTokeniser->Pop();

    int top = 0;
    int second = 0;
    int val = 0;

    switch (token.m_type)
    {
    case (PietToken::ETokenType::Push):
    {
        m_stack.Push(token.m_value);
        break;
    }
    case (PietToken::ETokenType::Pop):
    {
        if (m_stack.GetSize() > 0)
        {
            m_stack.Pop();
        }
        break;
    }
    case (PietToken::ETokenType::Add):
    {
        if (m_stack.GetSize() >= 2)
        {
            m_stack.Push(m_stack.Pop() + m_stack.Pop());
        }
        break;
    }
    case (PietToken::ETokenType::Subtract):
    {
        if (m_stack.GetSize() >= 2)
        {
            top = m_stack.Pop();
            second = m_stack.Pop();
            m_stack.Push(second - top);
        }
        break;
    }
    case (PietToken::ETokenType::Multiply):
    {
        if (m_stack.GetSize() >= 2)
        {
            m_stack.Push(m_stack.Pop() * m_stack.Pop());
        }
        break;
    }
    case (PietToken::ETokenType::Divide):
    {
        if (m_stack.GetSize() >= 2)
        {
            top = m_stack.Pop();
            second = m_stack.Pop();
            m_stack.Push(second / top);
        }
        break;
    }
    case (PietToken::ETokenType::Modulo):
    {
        if (m_stack.GetSize() >= 2)
        {
            top = m_stack.Pop();
            second = m_stack.Pop();
            m_stack.Push((top + (second % top)) % top);
        }
        break;
    }
    case (PietToken::ETokenType::Not):
    {
        if (m_stack.GetSize() > 0)
        {
            val = m_stack.Pop() == 0 ? 1 : 0;
            m_stack.Push(val);
        }
        break;
    }
    case (PietToken::ETokenType::Greater):
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
    case (PietToken::ETokenType::Pointer):
    {
        if (!m_stack.Empty())
        {
            top = m_stack.Pop();
            m_imageTokeniser.RotateDirectionPointer(top);
        }
        break;
    }
    case (PietToken::ETokenType::Switch):
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
    case (PietToken::ETokenType::Duplicate):
    {
        if (m_stack.GetSize() > 0)
        {
            val = m_stack.Pop();
            m_stack.Push(val);
            m_stack.Push(val);
        }
        break;
    }
    case (PietToken::ETokenType::Roll):
    {
        if (m_stack.GetSize() >= 2)
        {
            top = m_stack.Pop();
            second = m_stack.Pop();
            m_stack.Roll(second, top);
        }
        break;
    }
    case (PietToken::ETokenType::Input_Char):
    {
        m_waitingForCharInput = true;
        break;
    }
    case (PietToken::ETokenType::Input_Val):
    {
        m_waitingForValInput = true;
        break;
    }
    case (PietToken::ETokenType::Output_Char):
    {
        if (m_stack.GetSize() > 0)
        {
            m_rOutputStream << (char)m_stack.Pop();
        }
        break;
    }
    case (PietToken::ETokenType::Output_Val):
    {
        if (m_stack.GetSize() > 0)
        {
            m_rOutputStream << std::to_string(m_stack.Pop());
        }
        break;
    }
    case (PietToken::ETokenType::EnterSlide):
    {
        return PietToken::ETokenType::EnterSlide;
    }
    case (PietToken::ETokenType::ExitSlide):
    {
        return PietToken::ETokenType::ExitSlide;
    }
    case (PietToken::ETokenType::End):
    {
        m_bIsRunning = false;
        break;
    }
    default:
        break;
    }

    return token;
}

//////////////////////////////////////////////////////////////
void PietRuntime::RenderImageDisplay()
{
    if (ImGui::Begin("Piet Image"))
    {
        if (m_pTexture)
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
            RequestReset();
            m_bForceImage = true;
            m_currentSourceType = ESourceType::Image;
            m_activeTokeniser = (TPietTokeniser*)&m_imageTokeniser;
            m_rSync.m_iterations = -1;
        }

        ImGui::SameLine();
        {
            int currentinstructionWaitTime = m_rSync.m_instructionWaitTime.load();
            float newInstructionWaitTime = currentinstructionWaitTime / 1000.f;
            ImGui::SliderFloat("##ExecutionSpeed", &newInstructionWaitTime, 0, 3);
            m_rSync.m_instructionWaitTime.compare_exchange_strong(currentinstructionWaitTime, static_cast<int>(newInstructionWaitTime * 1000.f));
        }

        if (m_rSync.m_iterations == -1)
        {
            ImGui::SameLine();
            if (ImGui::Button("Pause"))
            {
                m_rSync.m_iterations = 0;
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Step"))
        {
            if (m_currentSourceType != ESourceType::Image)
            {
                m_bForceImage = true;
                RequestReset();
                m_currentSourceType = ESourceType::Image;
                m_activeTokeniser = (TPietTokeniser*)&m_imageTokeniser;
            }
            ++m_rSync.m_iterations;
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
            PietImageTokeniser::s_directionIcons[static_cast<int>(m_imageTokeniser.GetCachedBlockStartDirectionPointer())],
            PietImageTokeniser::s_directionIcons[static_cast<int>(m_imageTokeniser.GetCachedBlockEndDirectionPointer())]);
        ImGui::Text("Current Block Start CC: %s - End CC: %s",
            PietImageTokeniser::s_directionIcons[static_cast<int>(m_imageTokeniser.GetCachedBlockStartCodelChoser())],
            PietImageTokeniser::s_directionIcons[static_cast<int>(m_imageTokeniser.GetCachedBlockEndCodelChoser())]);

        ImGui::End();
    }
}