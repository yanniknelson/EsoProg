#pragma once

#include <IToken.h>
#include <iostream>
#include <SmartEnums.h>

class PietToken : public IToken
{
public:

#define ETOKENTYPE(x)\
    x(Start)\
	x(Value)\
	x(Push)\
	x(Pop)\
	x(Add)\
	x(Subtract)\
	x(Multiply)\
	x(Divide)\
	x(Modulo)\
	x(Not)\
	x(Greater)\
	x(Pointer)\
	x(Switch)\
	x(Duplicate)\
	x(Roll)\
	x(Input)\
	x(Output)\
	x(INT)\
	x(CHAR)\
	x(Input_Char)\
	x(Input_Val)\
	x(Output_Char)\
	x(Output_Val)\
	x(NOP)\
	x(End)\
	x(Unrecognised_Token)

	CreateSmartEnum(TokenType, ETOKENTYPE);

#undef ETOKENTYPE

	PietToken(const TokenType::Enum type, const int value) : m_type(type), m_value(value) {};
	PietToken(const TokenType::Enum type) : m_type(type), m_value(NAN) {};

	TokenType::Enum m_type = TokenType::Start;
	int m_value;

	/// <summary>
	/// Format and output the Token of the stack for and output stream
	/// </summary>
	/// <param name="os - "> The output stream </param>
	/// <param name="tk - "> The token to be output </param>
	/// <returns></returns>
	friend std::ostream& operator<<(std::ostream& os, const PietToken& tk)
	{
		os << PietToken::TokenType::ToString(tk.m_type);
		if(tk.m_type == TokenType::Push)
		{
			os << " " << tk.m_value;
		}
		return os;
	}
};