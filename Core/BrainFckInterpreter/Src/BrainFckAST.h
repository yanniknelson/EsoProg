#pragma once

#include <IAbstractSyntaxTree.h>

class LeftOp : public IOperation
{
};

class RightOp : public IOperation
{
};

class IncOp : public IOperation
{
};

class DecOp : public IOperation
{
};

class InOp : public IOperation
{
};

class OutOp : public IOperation
{
};

class Loop : public IOperation
{
public:
	Loop(IRegion pParent)
		: IOperation(pParent)
		, m_content(pParent)
	{
	}
	IRegion m_content;
};