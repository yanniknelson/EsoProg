#pragma once

#include "PietToken.h"

#include <ITextTokeniser.h>
#include <strstream>
#include <iostream>


class PietTextTokeniser : public ITextTokeniser<PietToken>
{
public:

	PietTextTokeniser() {};

private:

	/// <summary>
	/// Convert from string to TokenType enum
	/// </summary>
	/// <param name="rString - "> string to be converted </param>
	/// <returns> TokenType enum corresponding to input string </returns>
	PietToken::TokenType::Enum StringToTokenType(std::string& rString) const;

	PietToken GetNextToken();
	virtual PietToken Pop_Internal() override;
};