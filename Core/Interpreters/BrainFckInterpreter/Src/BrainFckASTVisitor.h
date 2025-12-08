#pragma once

#include <IASTVisitor.h>
#pragma once

#include<BrainFckAST.h>
#include<memory>
#include<iostream>

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