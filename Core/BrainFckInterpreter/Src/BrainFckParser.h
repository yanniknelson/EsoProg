#pragma once

#include <IParser.h>
#include <BrainFckToken.h>

class BrainFckParser: public IParser<BrainFckToken>
{
	virtual IRegion Parse() override;
	IOperation ParseExpression(IRegion* parentRegion);
	IOperation ParseOp(IRegion* parentRegion);
	IOperation ParseLoop(IRegion* parentRegion);
};