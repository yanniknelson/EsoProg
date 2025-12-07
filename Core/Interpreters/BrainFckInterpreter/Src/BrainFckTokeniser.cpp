#include "BrainFckTokeniser.h"

#include <iostream>
#include <sstream>

BrainFckToken BrainFckTokeniser::Pop_Internal()
{
	BrainFckToken token = GetNextToken();
	return token;
}

BrainFckToken BrainFckTokeniser::GetNextToken()
{
	char ch = ' ';
	BrainFckToken::TokenType::Enum currentTokenType = BrainFckToken::TokenType::Unrecognised_Token;
	std::string word;
	// ignore all white space and characters that aren't ><+-.,[]
	while (m_pStrStream->rdbuf()->in_avail() && (isspace(ch) || currentTokenType == BrainFckToken::TokenType::Unrecognised_Token))
	{
		m_pStrStream->get(ch);
		currentTokenType = CharToToken(ch);
		if (ch == '\n') { m_lineNumber++; }
	}

	if (currentTokenType == BrainFckToken::TokenType::Unrecognised_Token)
	{
		currentTokenType = BrainFckToken::TokenType::End;
	}
	
	return currentTokenType;
}

inline BrainFckToken::TokenType::Enum BrainFckTokeniser::CharToToken(const char chr) const
{
	switch (chr)
	{
	case('>'):
	{
		return BrainFckToken::TokenType::Move_Right;
	}
	case('<'):
	{
		return BrainFckToken::TokenType::Move_Left;
	}
	case('+'):
	{
		return BrainFckToken::TokenType::Increment;
	}
	case('-'):
	{
		return BrainFckToken::TokenType::Decrement;
	}
	case('.'):
	{
		return BrainFckToken::TokenType::Output_Char;
	}
	case(','):
	{
		return BrainFckToken::TokenType::Input_Char;
	}
	case('['):
	{
		return BrainFckToken::TokenType::Loop_Start;
	}
	case(']'):
	{
		return BrainFckToken::TokenType::Loop_End;
	}
	default:
	{
		return BrainFckToken::TokenType::Unrecognised_Token;
	}
	}
}