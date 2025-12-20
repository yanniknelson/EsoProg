#pragma once

#include <BrainFckAST.h>
#include <IASTVisitor.h>
#include <iostream>
#include <memory>

class BrainFckPrintingVisitor : public IASTVisitor<BrainFckProgram>
{
    using TProgramPtr = std::shared_ptr<BrainFckProgram>;

  public:
    virtual void Traverse(TProgramPtr pProgram) override;

    void Traverse(TBrainFckOperationPtr pOperation);
    void Visit(TBrainFckOperationPtr pOperation);
    void Traverse(std::shared_ptr<Loop> pLoop);

  private:
    size_t m_depth = 0;

    void Print(const char* str);
};

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
