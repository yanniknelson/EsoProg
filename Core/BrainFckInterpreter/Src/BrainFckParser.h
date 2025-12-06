#pragma once

#include <IParser.h>
#include <BrainFckToken.h>
#include "BrainFckAST.h"

class BrainFckParser: public IParser<BrainFckToken, BrainFckOperationTypes>
{
public:
	BrainFckParser(ITokeniser<BrainFckToken>* pTokeniser) : IParser(pTokeniser) {}
	virtual BrainFckOperation* Parse() override;
private:
	BrainFckOperation* ParseExpression(BrainFckOperation* parentRegion);
	BrainFckOperation* ParseOp(BrainFckOperation* parentRegion);
	BrainFckOperation* ParseLoop(BrainFckOperation* parentRegion);
};