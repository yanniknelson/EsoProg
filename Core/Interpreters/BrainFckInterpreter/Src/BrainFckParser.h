#pragma once

#include "BrainFckAST.h"    // for BrainFckOperationTypes, TBrainFckOperationPtr, TBrainFckRegionPtr
#include "BrainFckToken.h"  // for BrainFckToken

#include <IParser.h>        // for IParser
#include <ITokeniser.h>     // for ITokeniser

//////////////////////////////////////////////////////////////
class BrainFckParser : public IParser<BrainFckToken, BrainFckOperationTypes>
{
  public:
    BrainFckParser(ITokeniser<BrainFckToken>* pTokeniser);

  private:
    virtual TBrainFckOperationPtr Parse_Internal() override;
    TBrainFckOperationPtr ParseExpression(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
    TBrainFckOperationPtr ParseOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
    TBrainFckOperationPtr ParseLoop(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
};
