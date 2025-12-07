#pragma once

#include <IParser.h>
#include <BrainFckToken.h>
#include "BrainFckAST.h"

class BrainFckParser: public IParser<BrainFckToken, BrainFckOperationTypes>
{
public:
	BrainFckParser(ITokeniser<BrainFckToken>* pTokeniser) : IParser(pTokeniser) {}
	virtual BrainFckOperationPtr Parse() override;
private:
	BrainFckOperationPtr ParseExpression(BrainFckOperationPtr parentRegion);
	BrainFckOperationPtr ParseOp(BrainFckOperationPtr parentRegion);
	BrainFckOperationPtr ParseLoop(BrainFckOperationPtr parentRegion);
};