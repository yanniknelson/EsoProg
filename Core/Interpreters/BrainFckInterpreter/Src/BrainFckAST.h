#pragma once

#include <IAST.h>        // for IOperation, IProgram, IRegion
#include <SmartEnums.h>  // for CreateSmartEnums

#include <memory>

// clang-format off
#define BRAINFCKOPERATIONS(x)\
    x(Program)               \
    x(LeftOp)                \
    x(RightOp)               \
    x(IncOp)                 \
    x(DecOp)                 \
    x(OutOp)                 \
    x(InOp)                  \
    x(Loop)                  \
    x(End)                   \
    x(Error)

CreateSmartEnum(BrainFckOperationTypes, BRAINFCKOPERATIONS);

#undef BRAINFCKOPERATIONS
// clang-format on

using TBrainFckOperation = IOperation<BrainFckOperationTypes>;
using TBrainFckOperationPtr = std::shared_ptr<TBrainFckOperation>;
using TBrainFckOperationWkPtr = std::weak_ptr<TBrainFckOperation>;
using TBrainFckRegion = IRegion<BrainFckOperationTypes>;
using TBrainFckRegionPtr = std::shared_ptr<TBrainFckRegion>;

//////////////////////////////////////////////////////////////
class BrainFckProgram : public IProgram<BrainFckOperationTypes>
{
  public:
    BrainFckProgram()
        : IProgram(nullptr, nullptr)
    {
    };
};

//////////////////////////////////////////////////////////////
class LeftOp : public TBrainFckOperation
{
  public:
    LeftOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pRegion)
        : IOperation(pParent, pRegion)
    {
    };
    
    virtual BrainFckOperationTypes::Enum GetType() const
    {
        return BrainFckOperationTypes::LeftOp;
    };
};

//////////////////////////////////////////////////////////////
class RightOp : public TBrainFckOperation
{
  public:
    RightOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pRegion)
        : IOperation(pParent, pRegion)
    {
    };

    virtual BrainFckOperationTypes::Enum GetType() const
    {
        return BrainFckOperationTypes::RightOp;
    };
};

//////////////////////////////////////////////////////////////
class IncOp : public TBrainFckOperation
{
  public:
    IncOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pRegion)
        : IOperation(pParent, pRegion)
    {
    };

    virtual BrainFckOperationTypes::Enum GetType() const
    {
        return BrainFckOperationTypes::IncOp;
    };
};

//////////////////////////////////////////////////////////////
class DecOp : public TBrainFckOperation
{
  public:
    DecOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pRegion)
        : IOperation(pParent, pRegion)
    {
    };

    virtual BrainFckOperationTypes::Enum GetType() const
    {
        return BrainFckOperationTypes::DecOp;
    };
};

//////////////////////////////////////////////////////////////
class InOp : public TBrainFckOperation
{
  public:
    InOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pRegion)
        : IOperation(pParent, pRegion) 
    {
    };

    virtual BrainFckOperationTypes::Enum GetType() const
    {
        return BrainFckOperationTypes::InOp;
    };
};

//////////////////////////////////////////////////////////////
class OutOp : public TBrainFckOperation
{
  public:
    OutOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pRegion)
        : IOperation(pParent, pRegion)
    {
    };

    virtual BrainFckOperationTypes::Enum GetType() const
    {
        return BrainFckOperationTypes::OutOp;
    };
};

//////////////////////////////////////////////////////////////
class Loop : public TBrainFckOperation
{
  public:
    Loop(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pRegion)
        : IOperation(pParent, pRegion)
    {
    };

    virtual BrainFckOperationTypes::Enum GetType() const
    {
        return BrainFckOperationTypes::Loop;
    };

    TBrainFckRegionPtr m_pRegion = std::make_shared<TBrainFckRegion>();
};

//////////////////////////////////////////////////////////////
class BrainFckError : public IError<BrainFckOperationTypes>
{
  public:
    BrainFckError(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pRegion)
        : IError(pParent, pRegion)
    {
    };
};
