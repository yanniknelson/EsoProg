#pragma once

#include <IAST.h>

#include <memory>
#include <iostream>

template<typename Operation>
class IASTVisitor
{
	using TOperationPtr = std::shared_ptr<Operation>;
public:
	virtual void Traverse(TOperationPtr pOperation) {};
	virtual void Visit(TOperationPtr pOperation) {};
};


// This is an example visitor implementation only, each visitor needs to be implemented like this uniquely
// with traverse and potentially visit functions for all operation types.
template<typename OperationType>
class IProgramPrintVisitor : public IASTVisitor<IProgram<OperationType>>
{
	using TProgram = IProgram<OperationType>;
	using TOperationPtr = std::shared_ptr<IOperation<OperationType>>;
	using TProgramPtr = std::shared_ptr<TProgram>;
public:
	virtual void Traverse(TProgramPtr pProgram) override
	{
		Visit(pProgram);
		for (TOperationPtr pOperation : pProgram->m_region)
		{
			Traverse(pOperation);
		}
	};

	virtual void Visit(TProgramPtr pOperation) override
	{
		std::cout << "Program" << std::endl;
	};

	void Traverse(TOperationPtr pOperation)
	{
		Visit(pOperation);
	}

	void Visit(TOperationPtr pOperation)
	{
		std::cout << OperationType::ToString(pOperation->GetType()) << std::endl;
	};
};