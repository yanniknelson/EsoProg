#include "BrainFckParser.h"
#include "BrainFckAST.h"

IRegion BrainFckParser::Parse()
{
	IRegion program(nullptr);

	if (!Check(BrainFckToken::TokenType::End))
	{
		program.AddOperation(ParseExpression(&program));
	}
	else
	{
		Error("Empty program can't be parsed");
	}

	while (!Check(BrainFckToken::TokenType::End))
	{
		program.AddOperation(ParseExpression(&program));
	}
	return program;
}

IOperation BrainFckParser::ParseExpression(IRegion* parentRegion)
{
	if (Check(BrainFckToken::TokenType::Loop_Start))
	{
		return ParseLoop(parentRegion);
	}
	return ParseOp(parentRegion);
}

IOperation BrainFckParser::ParseOp(IRegion* parentRegion)
{
	if (Check(BrainFckToken::TokenType::Move_Left))
	{
		Match(BrainFckToken::TokenType::Move_Left);
		return LeftOp(parentRegion);
	}

	if (Check(BrainFckToken::TokenType::Move_Right))
	{
		Match(BrainFckToken::TokenType::Move_Right);
		return RightOp(parentRegion);
	}

	if (Check(BrainFckToken::TokenType::Increment))
	{
		Match(BrainFckToken::TokenType::Increment);
		return IncOp(parentRegion);
	}

	if (Check(BrainFckToken::TokenType::Decrement))
	{
		Match(BrainFckToken::TokenType::Decrement);
		return DecOp(parentRegion);
	}

	if (Check(BrainFckToken::TokenType::Input_Char))
	{
		Match(BrainFckToken::TokenType::Input_Char);
		return InOp(parentRegion);
	}

	if (Check(BrainFckToken::TokenType::Output_Char))
	{
		Match(BrainFckToken::TokenType::Output_Char);
		return OutOp(parentRegion);
	}

	Error("Expected an operator token (<>+-.,)");
	return IError(parentRegion);
}

IOperation BrainFckParser::ParseLoop(IRegion* parentRegion)
{
	if (Match(BrainFckToken::TokenType::Loop_Start).m_type != BrainFckToken::TokenType::Unrecognised_Token)
	{
		Loop loop(parentRegion);
		if (!Check(BrainFckToken::TokenType::End))
		{
			loop.m_content.AddOperation(ParseExpression(&loop.m_content));
		}
		else
		{
			Error("Empty program can't be parsed");
		}

		while (!Check(BrainFckToken::TokenType::End))
		{
			loop.m_content.AddOperation(ParseExpression(&loop.m_content));
		}
		return loop;
	}
	return IError(parentRegion);
}