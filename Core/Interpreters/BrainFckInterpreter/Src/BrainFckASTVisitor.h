#pragma once

#include <IASTVisitor.h>
#include<BrainFckAST.h>
#include<memory>
#include<iostream>

class BrainFckPrintingVisitor : public IASTVisitor<BrainFckProgram>
{
	using TProgramPtr = std::shared_ptr<BrainFckProgram>;
public:
	virtual void Traverse(TProgramPtr pProgram) override
	{
		m_depth = 0;
		print("Program");
		for (BrainFckOperationPtr pOperation : pProgram->m_region)
		{
			Traverse(pOperation);
		}
		print("Program End");
	};

	void Traverse(BrainFckOperationPtr pOperation)
	{
		switch (pOperation->GetType())
		{
		case(BrainFckOperationTypes::Loop):
		{
			Traverse(std::dynamic_pointer_cast<Loop>(pOperation));
			break;
		}
		case(BrainFckOperationTypes::IncOp): [[fallthrough]];
		case(BrainFckOperationTypes::DecOp): [[fallthrough]];
		case(BrainFckOperationTypes::LeftOp): [[fallthrough]];
		case(BrainFckOperationTypes::RightOp): [[fallthrough]];
		case(BrainFckOperationTypes::InOp): [[fallthrough]];
		case(BrainFckOperationTypes::OutOp): [[fallthrough]];
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

	void Visit(BrainFckOperationPtr pOperation)
	{
		print(BrainFckOperationTypes::ToString(pOperation->GetType()));
	};

	void Traverse(std::shared_ptr<Loop> pLoop)
	{
		print("Loop Start");
		m_depth += 1;
		for (BrainFckOperationPtr pOperation : pLoop->m_region)
		{
			Traverse(pOperation);
		}
		m_depth -= 1;
		print("Loop End");
	}
private:
	size_t m_depth = 0;

	void print(const char* str)
	{
		for (size_t i = 0; i < m_depth; i++)
		{
			std::cout << "\t";
		}
		std::cout << str << "\n";
	}
};