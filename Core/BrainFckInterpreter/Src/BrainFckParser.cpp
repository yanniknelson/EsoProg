#include "BrainFckParser.h"
#include "BrainFckAST.h"

BrainFckOperation* BrainFckParser::Parse()
{
	Program* pProgram = new Program(nullptr);

	if (!Check(BrainFckToken::TokenType::End))
	{
		pProgram->m_content.AddOperation(ParseExpression(nullptr));
	}
	else
	{
		Error("Empty program can't be parsed");
	}

	while (!Check(BrainFckToken::TokenType::End))
	{
		pProgram->m_content.AddOperation(ParseExpression(nullptr));
	}
	return pProgram;
}

BrainFckOperation* BrainFckParser::ParseExpression(BrainFckOperation* parentRegion)
{
	if (Check(BrainFckToken::TokenType::Loop_Start))
	{
		return ParseLoop(parentRegion);
	}
	return ParseOp(parentRegion);
}

BrainFckOperation* BrainFckParser::ParseOp(BrainFckOperation* pParent)
{
	if (Check(BrainFckToken::TokenType::Move_Left))
	{
		Match(BrainFckToken::TokenType::Move_Left);
		return new LeftOp(pParent);
	}

	if (Check(BrainFckToken::TokenType::Move_Right))
	{
		Match(BrainFckToken::TokenType::Move_Right);
		return new RightOp(pParent);
	}

	if (Check(BrainFckToken::TokenType::Increment))
	{
		Match(BrainFckToken::TokenType::Increment);
		return new IncOp(pParent);
	}

	if (Check(BrainFckToken::TokenType::Decrement))
	{
		Match(BrainFckToken::TokenType::Decrement);
		return new DecOp(pParent);
	}

	if (Check(BrainFckToken::TokenType::Input_Char))
	{
		Match(BrainFckToken::TokenType::Input_Char);
		return new InOp(pParent);
	}

	if (Check(BrainFckToken::TokenType::Output_Char))
	{
		Match(BrainFckToken::TokenType::Output_Char);
		return new OutOp(pParent);
	}

	Error("Expected an operator token (<>+-.,)");
	return new BrainFckError(pParent);
}

BrainFckOperation* BrainFckParser::ParseLoop(BrainFckOperation* pParent)
{
	if (Match(BrainFckToken::TokenType::Loop_Start).m_type != BrainFckToken::TokenType::Unrecognised_Token)
	{
		Loop* pLoop = new Loop(pParent);
		if (!Check(BrainFckToken::TokenType::End))
		{
			pLoop->m_content.AddOperation(ParseExpression(pLoop));
		}
		else
		{
			Error("Empty program can't be parsed");
			delete pLoop;
			return new BrainFckError(pParent);
		}

		while (!Check(BrainFckToken::TokenType::End))
		{
			if (Check(BrainFckToken::TokenType::Loop_End))
			{
				Match(BrainFckToken::TokenType::Loop_End);
				break;
			}
			pLoop->m_content.AddOperation(ParseExpression(pLoop));
		}
		return pLoop;
	}
	return new BrainFckError(pParent);
}