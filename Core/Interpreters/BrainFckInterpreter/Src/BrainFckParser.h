#pragma once

#include <IParser.h>
#include <BrainFckToken.h>
#include "BrainFckAST.h"

class BrainFckParser: public IParser<BrainFckToken, BrainFckOperationTypes>
{
public:
	BrainFckParser(ITokeniser<BrainFckToken>* pTokeniser) : IParser(pTokeniser) {}
	virtual TBrainFckOperationPtr Parse() override;
private:
	TBrainFckOperationPtr ParseExpression(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
	TBrainFckOperationPtr ParseOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
	TBrainFckOperationPtr ParseLoop(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion);
};