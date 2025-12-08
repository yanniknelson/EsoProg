#include "BrainFckASTVisitor.h"

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