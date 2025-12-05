#pragma once

#include <IAbstractSyntaxTree.h>

class LeftOp : public IOperation
{
};

class RightOp : public IOperation
{
};

class PlusOp : public IOperation
{
};

class NegOp : public IOperation
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
protected:
	IRegion m_content;
};