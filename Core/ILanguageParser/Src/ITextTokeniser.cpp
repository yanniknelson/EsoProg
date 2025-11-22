#include "ITextTokeniser.h"

#include <iostream>
#include <sstream>

template<typename TokenClass>
const TokenClass& ITextTokeniser<TokenClass>::Pop()
{
	TokenClass token = Pop_Internal();
	if (token.m_type == PietToken::TokenType::Push)
	{
		PietToken value = Pop_Internal();
		if (value.m_type != PietToken::TokenType::Value)
		{
			__debugbreak();
		}
		token.m_value = value.m_value;
	}
	return token;
}

template<typename TokenClass>
TokenClass ITextTokeniser<TokenClass>::Pop_Internal()
{
	if (!m_pStrStream->rdbuf()->in_avail())
	{
		m_tLastPopped.m_type = PietToken::TokenType::End;
		m_tLastPopped.m_value = NAN;
		return m_tLastPopped;
	}
	char ch = ' ';
	std::string word;
	while (isspace(ch))
	{
		m_pStrStream->get(ch);
		if (ch == '\n') { m_lineNumber++; }
	}

	if (isdigit(ch))
	{
		m_pStrStream->putback(ch);
		m_tLastPopped.m_type = PietToken::TokenType::Value;
		*m_pStrStream >> m_tLastPopped.m_value;
	}
	else
	{
		m_pStrStream->putback(ch);
		*m_pStrStream >> word;
		m_tLastPopped.m_type = StringToTokenType(word);
		m_tLastPopped.m_value = NAN;

		if (m_tLastPopped.m_type == PietToken::TokenType::CHAR || m_tLastPopped.m_type == PietToken::TokenType::INT)
		{
			m_tLastPopped.m_type = PietToken::TokenType::Unrecognised_Token;
		}

		if (m_tLastPopped.m_type == PietToken::TokenType::Input)
		{
			word = "";
			*m_pStrStream >> word;
			m_tLastPopped.m_type = StringToTokenType(word);

			if (m_tLastPopped.m_type == PietToken::TokenType::CHAR)
			{
				m_tLastPopped.m_type = PietToken::TokenType::Input_Char;
			}
			else if (m_tLastPopped.m_type == PietToken::TokenType::INT)
			{
				m_tLastPopped.m_type = PietToken::TokenType::Input_Val;
			}
			else
			{
				m_tLastPopped.m_type = PietToken::TokenType::Unrecognised_Token;
			}
		}
		else if (m_tLastPopped.m_type == PietToken::TokenType::Output)
		{
			word = "";
			*m_pStrStream >> word;
			m_tLastPopped.m_type = StringToTokenType(word);

			if (m_tLastPopped.m_type == PietToken::TokenType::CHAR)
			{
				m_tLastPopped.m_type = PietToken::TokenType::Output_Char;
			}
			else if (m_tLastPopped.m_type == PietToken::TokenType::INT)
			{
				m_tLastPopped.m_type = PietToken::TokenType::Output_Val;
			}
			else
			{
				m_tLastPopped.m_type = PietToken::TokenType::Unrecognised_Token;
			}
		}

	}

	return m_tLastPopped;
}

template<typename TokenClass>
TokenClass::TokenType ITextTokeniser<TokenClass>::StringToTokenType(std::string& rString) const
{
	if (rString == "PUSH")
	{
		return PietToken::TokenType::Push;
	}
	else if (rString == "POP")
	{
		return PietToken::TokenType::Pop;
	}
	else if (rString == "Add")
	{
		return PietToken::TokenType::Add;
	}
	else if (rString == "SUB")
	{
		return PietToken::TokenType::Subtract;
	}
	else if (rString == "MUL")
	{
		return PietToken::TokenType::Multiply;
	}
	else if (rString == "DIV")
	{
		return PietToken::TokenType::Divide;
	}
	else if (rString == "MOD")
	{
		return PietToken::TokenType::Modulo;
	}
	else if (rString == "NOT")
	{
		return PietToken::TokenType::Not;
	}
	else if (rString == "GR")
	{
		return PietToken::TokenType::Greater;
	}
	else if (rString == "PNTR")
	{
		return PietToken::TokenType::Pointer;
	}
	else if (rString == "SWTCH")
	{
		return PietToken::TokenType::Switch;
	}
	else if (rString == "DUP")
	{
		return PietToken::TokenType::Duplicate;
	}
	else if (rString == "ROLL")
	{
		return PietToken::TokenType::Roll;
	}
	else if (rString == "END")
	{
		return PietToken::TokenType::End;
	}
	else if (rString == "OUT")
	{
		return PietToken::TokenType::Output;
	}
	else if (rString == "IN")
	{
		return PietToken::TokenType::Input;
	}
	else if (rString == "CHAR")
	{
		return PietToken::TokenType::CHAR;
	}
	else if (rString == "INT")
	{
		return PietToken::TokenType::INT;
	}
	else
	{
		return PietToken::TokenType::Unrecognised_Token;
	}
}