#include "BrainFckRuntime.h"

#include "BrainFckAST.h"         // for BrainFckOperationTypes, BrainFckProgram
#include "BrainFckAstVisitor.h"  // for BrainFckPrintingVisitor

#include <CRuntime.h>            // for CRuntime
#include <IRuntime.h>            // for SRuntimeSyncronisationStruct
#include <ELanguages.h>          // for ELanguages::Enum

#include <sstream>               // for std::ostringstream
#include <iostream>
#include <memory>
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////
BrainFckRuntime::BrainFckRuntime(SRuntimeSyncronisationStruct& rSync, std::ostringstream& rOutputStream, std::ostringstream& rExecutionhistoryStream)
    : CRuntime(rSync, rOutputStream, rExecutionhistoryStream), m_parser(&m_tokeniser)
{
    m_tokeniser.SetTextStream(m_code);
};

//////////////////////////////////////////////////////////////
ELanguages::Enum BrainFckRuntime::GetRuntimeLanguage() const
{
    return ELanguages::Brainfck;
}

//////////////////////////////////////////////////////////////
std::vector<std::string> BrainFckRuntime::GetSupportedFileTypes() const
{
    return { ".txt" };
}

//////////////////////////////////////////////////////////////
void BrainFckRuntime::ResetImplementation()
{
    m_array.Clear();
}

//////////////////////////////////////////////////////////////
void BrainFckRuntime::RenderWindows()
{
    m_cachedArray.DisplayArray(m_cachedIndex);
}

//////////////////////////////////////////////////////////////
void BrainFckRuntime::CacheState()
{
    m_cachedIndex = m_currentIndex;
    m_cachedArray = m_array;
}

//////////////////////////////////////////////////////////////
void BrainFckRuntime::OnInput(int val)
{
    m_array.Set(m_currentIndex, val);
}

//////////////////////////////////////////////////////////////
void BrainFckRuntime::OnSourceSet()
{
    m_pProgramAST = std::dynamic_pointer_cast<BrainFckProgram>(m_parser.Parse());
    BrainFckPrintingVisitor().Traverse(m_pProgramAST);
    m_runtimeVisitor.SetProgram(m_pProgramAST);
}

//////////////////////////////////////////////////////////////
bool BrainFckRuntime::ShouldEnd(const BrainFckOperationTypes::Enum& token)
{
    return token == BrainFckOperationTypes::End || token == BrainFckOperationTypes::Error;
}

//////////////////////////////////////////////////////////////
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