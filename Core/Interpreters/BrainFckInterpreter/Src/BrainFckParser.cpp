#include "BrainFckParser.h"
#include "BrainFckAST.h"

BrainFckOperationPtr BrainFckParser::Parse()
{
	std::shared_ptr<Program> pProgram = std::make_shared<Program>(nullptr);

	if (!Check(BrainFckToken::TokenType::End))
	{
		pProgram->m_region.AddOperation(ParseExpression(nullptr));
	}
	else
	{
		Error("Empty program can't be parsed");
	}

	while (!Check(BrainFckToken::TokenType::End))
	{
		pProgram->m_region.AddOperation(ParseExpression(nullptr));
	}
	return pProgram;
}

BrainFckOperationPtr BrainFckParser::ParseExpression(BrainFckOperationPtr parentRegion)
{
	if (Check(BrainFckToken::TokenType::Loop_Start))
	{
		return ParseLoop(parentRegion);
	}
	return ParseOp(parentRegion);
}

BrainFckOperationPtr BrainFckParser::ParseOp(BrainFckOperationPtr pParent)
{
	if (Check(BrainFckToken::TokenType::Move_Left))
	{
		Match(BrainFckToken::TokenType::Move_Left);
		return std::make_shared<LeftOp>(pParent);
	}

	if (Check(BrainFckToken::TokenType::Move_Right))
	{
		Match(BrainFckToken::TokenType::Move_Right);
		return std::make_shared<RightOp>(pParent);
	}

	if (Check(BrainFckToken::TokenType::Increment))
	{
		Match(BrainFckToken::TokenType::Increment);
		return std::make_shared<IncOp>(pParent);
	}

	if (Check(BrainFckToken::TokenType::Decrement))
	{
		Match(BrainFckToken::TokenType::Decrement);
		return std::make_shared<DecOp>(pParent);
	}

	if (Check(BrainFckToken::TokenType::Input_Char))
	{
		Match(BrainFckToken::TokenType::Input_Char);
		return std::make_shared<InOp>(pParent);
	}

	if (Check(BrainFckToken::TokenType::Output_Char))
	{
		Match(BrainFckToken::TokenType::Output_Char);
		return std::make_shared<OutOp>(pParent);
	}

	Error("Expected an operator token (<>+-.,)");
	return std::make_shared<BrainFckError>(pParent);
}

BrainFckOperationPtr BrainFckParser::ParseLoop(BrainFckOperationPtr pParent)
{
	if (Match(BrainFckToken::TokenType::Loop_Start).m_type != BrainFckToken::TokenType::Unrecognised_Token)
	{
		std::shared_ptr<Loop> pLoop = std::make_shared<Loop>(pParent);
		if (!Check(BrainFckToken::TokenType::End))
		{
			pLoop->m_region.AddOperation(ParseExpression(pLoop));
		}
		else
		{
			Error("Empty program can't be parsed");
			return std::make_shared<BrainFckError>(pParent);
		}

		while (!Check(BrainFckToken::TokenType::End))
		{
			if (Check(BrainFckToken::TokenType::Loop_End))
			{
				Match(BrainFckToken::TokenType::Loop_End);
				break;
			}
			pLoop->m_region.AddOperation(ParseExpression(pLoop));
		}
		return pLoop;
	}
	return std::make_shared<BrainFckError>(pParent);
}