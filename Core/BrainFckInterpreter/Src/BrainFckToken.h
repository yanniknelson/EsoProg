#pragma once

#include <IToken.h>
#include <iostream>
#include <SmartEnums.h>

class BrainFckToken : public IToken
{
public:

#define ETOKENTYPE(x)\
    x(Start)\
	x(Move_Left)\
	x(Move_Right)\
	x(Increment)\
	x(Decrement)\
	x(Output_Char)\
	x(Input_Char)\
	x(Loop_Start)\
	x(Loop_End)\
	x(NOP)\
	x(End)\
	x(Unrecognised_Token)

	CreateSmartEnum(TokenType, ETOKENTYPE);

#undef ETOKENTYPE

	BrainFckToken(const TokenType::Enum type) : m_type(type) {};

	TokenType::Enum m_type = TokenType::Start;

	/// <summary>
	/// Format and output the Token of the stack for and output stream
	/// </summary>
	/// <param name="os - "> The output stream </param>
	/// <param name="tk - "> The token to be output </param>
	/// <returns></returns>
	friend std::ostream& operator<<(std::ostream& os, const BrainFckToken& tk)
	{
		os << BrainFckToken::TokenType::ToString(tk.m_type);
		return os;
	}
};