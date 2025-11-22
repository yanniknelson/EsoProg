#pragma once

#include "PietTokeniser.h"

#include <strstream>
#include <iostream>


class PietTextTokeniser : public PietTokeniser
{
public:

	/// <summary>
	/// Get the next token
	/// </summary>
	/// <returns> The next token </returns>
	virtual const PietToken& Pop() override;

	PietTextTokeniser() {};

	void SetTextStream(std::stringstream& m_code)
	{
		m_pStrStream = &m_code;
		m_lineNumber = 1;
	}

	int GetLineNumber()
	{
		return m_lineNumber;
	}

private:

	/// <summary>
	/// Convert from string to TokenType enum
	/// </summary>
	/// <param name="rString - "> string to be converted </param>
	/// <returns> TokenType enum corresponding to input string </returns>
	PietToken::TokenType StringToTokenType(std::string& rString) const;

	PietToken Pop_Internal();

	std::stringstream* m_pStrStream{ nullptr };
	PietToken m_tLastPopped{ PietToken::TokenType::End };
	int m_lineNumber = 1;

};