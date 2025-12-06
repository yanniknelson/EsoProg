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
using BrainFckOperationPtr = std::shared_ptr<BrainFckOperation>;
using BrainFckRegion = IRegion<BrainFckOperationTypes>;

class Program : public BrainFckOperation
{
public:
	Program(BrainFckOperationPtr pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::Loop; };
	BrainFckRegion m_region;
};

class LeftOp : public BrainFckOperation
{
public:
	LeftOp(BrainFckOperationPtr pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::LeftOp; };
};

class RightOp : public BrainFckOperation
{
public:
	RightOp(BrainFckOperationPtr pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::RightOp; };
};

class IncOp : public BrainFckOperation
{
public:
	IncOp(BrainFckOperationPtr pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::IncOp; };
};

class DecOp : public BrainFckOperation
{
public:
	DecOp(BrainFckOperationPtr pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::DecOp; };
};

class InOp : public BrainFckOperation
{
public:
	InOp(BrainFckOperationPtr pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::InOp; };
};

class OutOp : public BrainFckOperation
{
public:
	OutOp(BrainFckOperationPtr pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::OutOp; };
};

class Loop : public BrainFckOperation
{
public:
	Loop(BrainFckOperationPtr pParent) : IOperation(pParent) {};
	virtual BrainFckOperationTypes::Enum GetType() const { return BrainFckOperationTypes::Loop; };
	BrainFckRegion m_region;
};

class BrainFckError : public IError<BrainFckOperationTypes>
{
public:
	BrainFckError(BrainFckOperationPtr pParent) : IError(pParent) {};
};