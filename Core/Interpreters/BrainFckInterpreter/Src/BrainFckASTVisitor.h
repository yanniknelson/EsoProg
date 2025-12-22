#pragma once

#include <BrainFckAST.h>  // for BrainFckOperationTypes, BrainFckProgram, TBrainFckOperationPtr, Loop
#include <IASTVisitor.h>  // for IASTVisitor

#include <cstdint>        // for uint8_t
#include <memory>

//////////////////////////////////////////////////////////////
class BrainFckPrintingVisitor : public IASTVisitor<BrainFckProgram>
{
    using TProgramPtr = std::shared_ptr<BrainFckProgram>;

  public:
    // IASTVisitor
    virtual void Traverse(TProgramPtr pProgram) override;
    // ~IASTVisitor

    void Traverse(TBrainFckOperationPtr pOperation);
    void Visit(TBrainFckOperationPtr pOperation);
    void Traverse(std::shared_ptr<Loop> pLoop);

  private:
    size_t m_depth = 0;

    void Print(const char* str);
};

//////////////////////////////////////////////////////////////
class BrainFckRuntimeVisitor
{
    using TProgramPtr = std::shared_ptr<BrainFckProgram>;

  public:
    void SetProgram(TBrainFckOperationPtr pProgram);
    BrainFckOperationTypes::Enum Step(uint8_t currVal);
    void EnterRegion(uint8_t currVal);
    void ExitRegion(uint8_t currVal);
    void MoveToNextOp(uint8_t currVal);
    BrainFckOperationTypes::Enum GetOperationType();

  private:
    TBrainFckOperationWkPtr m_pCurrentOperation;
};
