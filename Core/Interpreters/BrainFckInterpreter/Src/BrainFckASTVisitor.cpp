#include "BrainFckASTVisitor.h"

#include "BrainFckAST.h"  // for TBrainFckOperationPtr, BrainFckOperationTypes

#include <LogLevel.h>       // for ETraceVerbosityLevel
#include <LogMacros.h>      // for LOG_INFO ....
#include <LogManager.h>     // for CLogmanager::GetOrCreate

#include <cstdint>        // for uint8_t
#include <iostream>
#include <memory>

//////////////////////////////////////////////////////////////
void BrainFckPrintingVisitor::Traverse(TProgramPtr pProgram)
{
    Print("Program");
    m_depth = 1;
    for (TBrainFckOperationPtr pOperation : *pProgram->m_pRegion)
    {
        Traverse(pOperation);
    }
    m_depth = 0;
    Print("Program End");
};

//////////////////////////////////////////////////////////////
void BrainFckPrintingVisitor::Traverse(TBrainFckOperationPtr pOperation)
{
    switch (pOperation->GetType())
    {
    case (BrainFckOperationTypes::Loop):
    {
        Traverse(std::dynamic_pointer_cast<Loop>(pOperation));
        break;
    }
    case (BrainFckOperationTypes::IncOp): [[fallthrough]];
    case (BrainFckOperationTypes::DecOp): [[fallthrough]];
    case (BrainFckOperationTypes::LeftOp): [[fallthrough]];
    case (BrainFckOperationTypes::RightOp): [[fallthrough]];
    case (BrainFckOperationTypes::InOp): [[fallthrough]];
    case (BrainFckOperationTypes::OutOp):
    {
        Visit(pOperation);
        break;
    }
    default:
    {
        std::cout << "Error" << std::endl;
    }
    }
}

//////////////////////////////////////////////////////////////
void BrainFckPrintingVisitor::Visit(TBrainFckOperationPtr pOperation)
{
    Print(BrainFckOperationTypes::ToString(pOperation->GetType()));
};

//////////////////////////////////////////////////////////////
void BrainFckPrintingVisitor::Traverse(std::shared_ptr<Loop> pLoop)
{
    Print("Loop Start");
    m_depth += 1;
    for (TBrainFckOperationPtr pOperation : *pLoop->m_pRegion)
    {
        Traverse(pOperation);
    }
    m_depth -= 1;
    Print("Loop End");
}

//////////////////////////////////////////////////////////////
void BrainFckPrintingVisitor::Print(const char* str)
{
    for (size_t i = 0; i < m_depth; i++)
    {
        std::cout << "\t";
    }
    std::cout << str << "\n";
}

BrainFckRuntimeVisitor::BrainFckRuntimeVisitor()
{
    m_pLogger = CLogManager::GetOrCreate("BrainFck-Runtime-Visitor", /*boutputToConsole =*/true, /*bOutputToFile =*/true, /*bOutputToUniqueFile =*/true);
    LOG_TRACE(m_pLogger, ETraceVerbosityLevel::Low, "BrainFck runtime visitor instance created");
}

//////////////////////////////////////////////////////////////
void BrainFckRuntimeVisitor::SetProgram(TBrainFckOperationPtr pProgram)
{
    m_pCurrentOperation = pProgram;
}

//////////////////////////////////////////////////////////////
BrainFckOperationTypes::Enum BrainFckRuntimeVisitor::Step(uint8_t currVal)
{
    LOG_TRACE(m_pLogger, ETraceVerbosityLevel::High, "stepping execution");
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();
    if (!pCurrentOperation)
    {
        LOG_TRACE(m_pLogger, ETraceVerbosityLevel::High, "No current operation, program is at end", BrainFckOperationTypes::ToString(pCurrentOperation->GetType()));
        return BrainFckOperationTypes::End;
    }

    BrainFckOperationTypes::Enum currOpType = GetOperationType();
    if (pCurrentOperation->GetType() == BrainFckOperationTypes::Loop || pCurrentOperation->GetType() == BrainFckOperationTypes::Program)
    {
        EnterRegion(currVal);
    }
    else
    {
        MoveToNextOp(currVal);
    }
    return currOpType;
}

//////////////////////////////////////////////////////////////
void BrainFckRuntimeVisitor::EnterRegion(uint8_t currVal)
{
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();

    if (pCurrentOperation->GetType() == BrainFckOperationTypes::Program)
    {
        std::shared_ptr<BrainFckProgram> pProgram = std::dynamic_pointer_cast<BrainFckProgram>(pCurrentOperation);
        m_pCurrentOperation = pProgram->m_pRegion->front();
        LOG_TRACE(m_pLogger, ETraceVerbosityLevel::High, "Exiting program region");

    }
    else if (pCurrentOperation->GetType() == BrainFckOperationTypes::Loop)
    {
        std::shared_ptr<Loop> pLoop = std::dynamic_pointer_cast<Loop>(pCurrentOperation);
        // if the current value is not zero then we enter the Loop's Region
        if (currVal != 0)
        {
            LOG_TRACE(m_pLogger, ETraceVerbosityLevel::High, "Restarting loop", BrainFckOperationTypes::ToString(pCurrentOperation->GetType()));
            m_pCurrentOperation = pLoop->m_pRegion->front();
        }
        // otherwise we skip the loop body and move on with execution
        else
        {
            LOG_TRACE(m_pLogger, ETraceVerbosityLevel::High, "Skipping loop region");
            MoveToNextOp(currVal);
        }
    }
}

//////////////////////////////////////////////////////////////
void BrainFckRuntimeVisitor::ExitRegion(uint8_t currVal)
{
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();
    LOG_TRACE(m_pLogger, ETraceVerbosityLevel::High, "Exiting {} region", BrainFckOperationTypes::ToString(pCurrentOperation->GetType()));
    TBrainFckOperationPtr pParentOperation = pCurrentOperation->GetParent();
    if (pParentOperation->GetType() == BrainFckOperationTypes::Program)
    {
        m_pCurrentOperation = pParentOperation->GetParent();
    }
    else if (pParentOperation->GetType() == BrainFckOperationTypes::Loop)
    {
        m_pCurrentOperation = pParentOperation;
    }
}

//////////////////////////////////////////////////////////////
void BrainFckRuntimeVisitor::MoveToNextOp(uint8_t currVal)
{
    LOG_TRACE(m_pLogger, ETraceVerbosityLevel::High, "Moving to next operation");
    //get the next instruction in the same region as the current instruction
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();
    if (pCurrentOperation)
    {
        const TBrainFckOperationPtr nextOp = pCurrentOperation->GetNextOperation();
        if (nextOp)
        {
            m_pCurrentOperation = nextOp;
            return;
        }
        // if there is no instruction after the loop then we need to exit the region of the the current instruction
        else
        {
            ExitRegion(currVal);
        }
    }
}

//////////////////////////////////////////////////////////////
BrainFckOperationTypes::Enum BrainFckRuntimeVisitor::GetOperationType()
{
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();
    if (pCurrentOperation)
    {
        return pCurrentOperation->GetType();
    }
    return BrainFckOperationTypes::End;
}
