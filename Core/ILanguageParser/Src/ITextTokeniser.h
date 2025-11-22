#pragma once

#include "ITokeniser.h"

#include <strstream>
#include <iostream>

template<typename TokenClass>
class ITextTokeniser : public ITokeniser<TokenClass>
{
public:

	/// <summary>
	/// Get the next token
	/// </summary>
	/// <returns> The next token </returns>
	virtual const TokenClass& Pop() override;

	ITextTokeniser() {};

	void SetTextStream(std::stringstream& m_code)
	{
		m_pStrStream = &m_code;
		m_lineNumber = 1;
	}

	int GetLineNumber()
	{
		return m_lineNumber;
	}

protected:

	/// <summary>
	/// Convert from string to TokenType enum
	/// </summary>
	/// <param name="rString - "> string to be converted </param>
	/// <returns> TokenType enum corresponding to input string </returns>
	TokenClass::TokenType StringToTokenType(std::string& rString) const;

	TokenClass Pop_Internal();

	std::stringstream* m_pStrStream{ nullptr };
	TokenClass m_tLastPopped{ TokenClass::TokenType::End };
	int m_lineNumber = 1;
};