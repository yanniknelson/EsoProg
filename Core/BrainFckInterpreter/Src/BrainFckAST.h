#pragma once

#include <IAbstractSyntaxTree.h>
#include <SmartEnums.h>

#define BRAINFCKOPERATIONS(x)\
	x(Program)\
	x(LeftOp)\
	x(RightOp)\
	x(IncOp)\
	x(DecOp)\
	x(OutOp)\
	x(InOp)\
	x(Loop)\
	x(Error)\

CreateSmartEnum(BrainFckOperationTypes, BRAINFCKOPERATIONS);


#undef BRAINFCKOPERATIONS

using BrainFckOperation = IOperation<BrainFckOperationTypes>;
using BrainFckRegion = IRegion<BrainFckOperationTypes>;

class Program : public BrainFckOperation
{
public:
	Program(BrainFckOperation* pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::Loop; };
	BrainFckRegion m_content;
};

class LeftOp : public BrainFckOperation
{
public:
	LeftOp(BrainFckOperation* pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::LeftOp; };
};

class RightOp : public BrainFckOperation
{
public:
	RightOp(BrainFckOperation* pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::RightOp; };
};

class IncOp : public BrainFckOperation
{
public:
	IncOp(BrainFckOperation* pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::IncOp; };
};

class DecOp : public BrainFckOperation
{
public:
	DecOp(BrainFckOperation* pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::DecOp; };
};

class InOp : public BrainFckOperation
{
public:
	InOp(BrainFckOperation* pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::InOp; };
};

class OutOp : public BrainFckOperation
{
public:
	OutOp(BrainFckOperation* pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::OutOp; };
};

class Loop : public BrainFckOperation
{
public:
	Loop(BrainFckOperation* pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::Loop; };
	BrainFckRegion m_content;
};

class BrainFckError : public IError<BrainFckOperationTypes>
{
public:
	BrainFckError(BrainFckOperation* pParent) : IError(pParent) {};
};