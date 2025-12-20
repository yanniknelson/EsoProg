#pragma once

#include "BrainFckRuntime.h"
#include "BrainFckAstVisitor.h"
#include "BrainFckToken.h"

//ImGui imports
#include <imgui.h>
#include <imgui_stdlib.h>

//OpenGL imports
#include "GLFW/glfw3.h"

//Other imports
#include <fstream>
#include <iostream>
#include <stdio.h>

#include <ImGuiValueChangeCallbacks.h>

BrainFckOperationTypes::Enum BrainFckRuntime::StepExecution_Internal()
{
    BrainFckOperationTypes::Enum operation = m_runtimeVisitor.Step(m_array.Get(m_currentIndex));
    switch (operation)
    {
    case (BrainFckOperationTypes::RightOp):
    {
        ++m_currentIndex;
        break;
    }
    case (BrainFckOperationTypes::LeftOp):
    {
        --m_currentIndex;
        break;
    }
    case (BrainFckOperationTypes::IncOp):
    {
        m_array.Increment(m_currentIndex);
        break;
    }
    case (BrainFckOperationTypes::DecOp):
    {
        m_array.Decrement(m_currentIndex);
        break;
    }
    case (BrainFckOperationTypes::OutOp):
    {
        m_rOutputStream << (char)m_array.Get(m_currentIndex);
        break;
    }
    case (BrainFckOperationTypes::InOp):
    {
        m_waitingForCharInput = true;
        break;
    }
    case (BrainFckOperationTypes::End):
    {
        m_bIsRunning = false;
        break;
    }
    default:
        break;
    }

    return operation;
}

void BrainFckRuntime::OnSourceSet()
{
    m_pProgramAST = std::dynamic_pointer_cast<BrainFckProgram>(m_parser.Parse());
    BrainFckPrintingVisitor().Traverse(m_pProgramAST);
    m_runtimeVisitor.SetProgram(m_pProgramAST);
}

void BrainFckRuntime::OnInput(int val)
{
    m_array.Set(m_currentIndex, val);
}

bool BrainFckRuntime::ShouldEnd(const BrainFckOperationTypes::Enum& token)
{
    return token == BrainFckOperationTypes::End || token == BrainFckOperationTypes::Error;
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
