#include "BrainFckASTVisitor.h"

#include "BrainFckAST.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>

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
        [[fallthrough]];
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

void BrainFckPrintingVisitor::Visit(TBrainFckOperationPtr pOperation)
{
    Print(BrainFckOperationTypes::ToString(pOperation->GetType()));
};

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

void BrainFckPrintingVisitor::Print(const char* str)
{
    for (size_t i = 0; i < m_depth; i++)
    {
        std::cout << "\t";
    }
    std::cout << str << "\n";
}

void BrainFckRuntimeVisitor::SetProgram(TBrainFckOperationPtr pProgram)
{
    m_pCurrentOperation = pProgram;
}

BrainFckOperationTypes::Enum BrainFckRuntimeVisitor::Step(uint8_t currVal)
{
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();
    if (!pCurrentOperation)
        return BrainFckOperationTypes::End;

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

void BrainFckRuntimeVisitor::EnterRegion(uint8_t currVal)
{
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();

    if (pCurrentOperation->GetType() == BrainFckOperationTypes::Program)
    {
        std::shared_ptr<BrainFckProgram> pProgram = std::dynamic_pointer_cast<BrainFckProgram>(pCurrentOperation);
        m_pCurrentOperation = pProgram->m_pRegion->front();
    }
    else if (pCurrentOperation->GetType() == BrainFckOperationTypes::Loop)
    {
        std::shared_ptr<Loop> pLoop = std::dynamic_pointer_cast<Loop>(pCurrentOperation);
        // if the current value is not zero then we enter the Loop's Region
        if (currVal != 0)
        {
            m_pCurrentOperation = pLoop->m_pRegion->front();
        }
        // otherwise we skip the loop body and move on with execution
        else
        {
            MoveToNextOp(currVal);
        }
    }
}

void BrainFckRuntimeVisitor::ExitRegion(uint8_t currVal)
{
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();
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

void BrainFckRuntimeVisitor::MoveToNextOp(uint8_t currVal)
{
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

BrainFckOperationTypes::Enum BrainFckRuntimeVisitor::GetOperationType()
{
    TBrainFckOperationPtr pCurrentOperation = m_pCurrentOperation.lock();
    if (pCurrentOperation)
    {
        return pCurrentOperation->GetType();
    }
    return BrainFckOperationTypes::End;
}
