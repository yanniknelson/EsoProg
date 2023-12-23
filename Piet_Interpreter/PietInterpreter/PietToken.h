#pragma once

#include <iostream>

class PietToken
{

public:
	enum class TokenType
	{
		Start, Value, Push, Pop, Add, Subtract, Multiply, Divide, Modulo, Not, Greater,
		Pointer, Switch, Duplicate, Roll, Input, Output, INT, CHAR, Input_Char, Input_Val, Output_Char, Output_Val, End, Unrecognised_Token
	};

	TokenType m_kind = TokenType::Start;
	int m_value;

	/// <summary>
	/// Format and output the Token of the stack for and output stream
	/// </summary>
	/// <param name="os - "> The output stream </param>
	/// <param name="tk - "> The token to be output </param>
	/// <returns></returns>
	friend std::ostream& operator<<(std::ostream& os, const PietToken& tk)
	{
		switch (tk.m_kind)
		{
		case(TokenType::Value):
		{
			os << tk.m_value;
			break;
		}
		case(TokenType::Push):
		{
			os << "Push";
			break;
		}
		case(TokenType::Pop):
		{
			os << "Pop";
			break;
		}
		case(TokenType::Add):
		{
			os << "Add";
			break;
		}
		case(TokenType::Subtract):
		{
			os << "Subtract";
			break;
		}
		case(TokenType::Multiply):
		{
			os << "Multiply";
			break;
		}
		case(TokenType::Divide):
		{
			os << "Divide";
			break;
		}
		case(TokenType::Modulo):
		{
			os << "Modulo";
			break;
		}
		case(TokenType::Not):
		{
			os << "Not";
			break;
		}
		case(TokenType::Greater):
		{
			os << "Greater";
			break;
		}
		case(TokenType::Pointer):
		{
			os << "Pointer";
			break;
		}
		case(TokenType::Switch):
		{
			os << "Switch";
			break;
		}
		case(TokenType::Duplicate):
		{
			os << "Duplicate";
			break;
		}
		case(TokenType::Roll):
		{
			os << "Roll";
			break;
		}
		case(TokenType::Input_Char):
		{
			os << "Input_Char";
			break;
		}
		case(TokenType::Input_Val):
		{
			os << "Input_Val";
			break;
		}
		case(TokenType::Output_Char):
		{
			os << "Output_Char";
			break;
		}
		case(TokenType::Output_Val):
		{
			os << "Output_Val";
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