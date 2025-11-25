#pragma once

#include "BrainFckToken.h"

#include <ITextTokeniser.h>
#include <strstream>
#include <iostream>


class BrainFckTokeniser : public ITextTokeniser<BrainFckToken>
{
public:

	BrainFckTokeniser() {};

private:

	/// <summary>
	/// Convert from char to TokenType enum
	/// </summary>
	/// <param name="chr - "> char to be converted </param>
	/// <returns> TokenType enum corresponding to input char </returns>
	inline BrainFckToken::TokenType CharToToken(const char chr) const;

	BrainFckToken GetNextToken();
	virtual BrainFckToken Pop_Internal() override;
};