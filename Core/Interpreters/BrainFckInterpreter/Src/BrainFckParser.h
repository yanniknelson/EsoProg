#pragma once

#include "BrainFckAST.h"
#include <BrainFckToken.h>
#include <IParser.h>

class BrainFckParser : public IParser<BrainFckToken, BrainFckOperationTypes>
{
  public:
    BrainFckParser(ITokeniser<BrainFckToken>* pTokeniser) : IParser(pTokeniser)
    {
    }

  private:
    virtual TBrainFckOperationPtr Parse_Internal() override;
    TBrainFckOperationPtr ParseExpression(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
    TBrainFckOperationPtr ParseOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
    TBrainFckOperationPtr ParseLoop(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
};
