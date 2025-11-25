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
	BrainFckToken::TokenType currentTokenType = CharToToken(ch);
	std::string word;
	// ignore all white space and characters that aren't ><+-.,[]
	while (m_pStrStream->rdbuf()->in_avail() || isspace(ch) || currentTokenType == BrainFckToken::TokenType::Unrecognised_Token)
	{
		m_pStrStream->get(ch);
		if (ch == '\n') { m_lineNumber++; }
	}

	if (!m_pStrStream->rdbuf()->in_avail())
	{
		m_tLastPopped.m_type = BrainFckToken::TokenType::End;
		return m_tLastPopped;
	}
	
	m_tLastPopped.m_type = currentTokenType;

	return m_tLastPopped;
}

inline BrainFckToken::TokenType BrainFckTokeniser::CharToToken(const char chr) const
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
		return BrainFckToken::TokenType::Branch_Start;
	}
	case(']'):
	{
		return BrainFckToken::TokenType::Branch_End;
	}
	default:
	{
		return BrainFckToken::TokenType::Unrecognised_Token;
	}
	}
}