#pragma once

#include <IToken.h>
#include <iostream>

class BrainFckToken : public IToken
{
public:
	enum class TokenType
	{
		Start, Move_Left, Move_Right, Increment, Decrement, Output_Char, Input_Char, Branch_Start, Branch_End, NOP, End, Unrecognised_Token
	};

	BrainFckToken(const TokenType type) : m_type(type) {};

	TokenType m_type = TokenType::Start;

	/// <summary>
	/// Format and output the Token of the stack for and output stream
	/// </summary>
	/// <param name="os - "> The output stream </param>
	/// <param name="tk - "> The token to be output </param>
	/// <returns></returns>
	friend std::ostream& operator<<(std::ostream& os, const BrainFckToken& tk)
	{
		switch (tk.m_type)
		{
		case(TokenType::Move_Left):
		{
			os << "Move_Left";
			break;
		}
		case(TokenType::Move_Right):
		{
			os << "Move_Right";
			break;
		}
		case(TokenType::Increment):
		{
			os << "Increment";
			break;
		}
		case(TokenType::Decrement):
		{
			os << "Decrement";
			break;
		}
		case(TokenType::Output_Char):
		{
			os << "Output_Char";
			break;
		}
		case(TokenType::Input_Char):
		{
			os << "Input_Char";
			break;
		}
		case(TokenType::Branch_Start):
		{
			os << "Branch_Start";
			break;
		}
		case(TokenType::Branch_End):
		{
			os << "Branch_End";
			break;
		}
		case(TokenType::NOP):
		{
			os << "NOP";
			break;
		}
		case(TokenType::End):
		{
			os << "End";
			break;
		}
		case(TokenType::Unrecognised_Token):
		{
			os << "Unrecognised_Token";
			break;
		}
		default:
			break;
		}
		return os;
	}
};