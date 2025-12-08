#include "BrainFckASTVisitor.h"

void BrainFckPrintingVisitor::Traverse(TProgramPtr pProgram)
{
	Print("Program");
	m_depth = 1;
	for (BrainFckOperationPtr pOperation : pProgram->m_region)
	{
		Traverse(pOperation);
	}
	m_depth = 0;
	Print("Program End");
};

void BrainFckPrintingVisitor::Traverse(BrainFckOperationPtr pOperation)
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

void BrainFckPrintingVisitor::Visit(BrainFckOperationPtr pOperation)
{
	Print(BrainFckOperationTypes::ToString(pOperation->GetType()));
};

void BrainFckPrintingVisitor::Traverse(std::shared_ptr<Loop> pLoop)
{
	Print("Loop Start");
	m_depth += 1;
	for (BrainFckOperationPtr pOperation : pLoop->m_region)
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