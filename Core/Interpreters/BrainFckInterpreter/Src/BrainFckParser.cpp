#include "BrainFckParser.h"
#include "BrainFckAST.h"

TBrainFckOperationPtr BrainFckParser::Parse()
{
	std::shared_ptr<BrainFckProgram> pProgram = std::make_shared<BrainFckProgram>();

	if (!Check(BrainFckToken::TokenType::End))
	{
		pProgram->m_pRegion->AddOperation(ParseExpression(pProgram, pProgram->m_pRegion));
	}
	else
	{
		Error("Empty program can't be parsed");
	}

	while (!Check(BrainFckToken::TokenType::End))
	{
		pProgram->m_pRegion->AddOperation(ParseExpression(pProgram, pProgram->m_pRegion));
	}
	return pProgram;
}

TBrainFckOperationPtr BrainFckParser::ParseExpression(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion)
{
	if (Check(BrainFckToken::TokenType::Loop_Start))
	{
		return ParseLoop(pParent, pParentRegion);
	}
	return ParseOp(pParent, pParentRegion);
}

TBrainFckOperationPtr BrainFckParser::ParseOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion)
{
	if (Check(BrainFckToken::TokenType::Move_Left))
	{
		Match(BrainFckToken::TokenType::Move_Left);
		return std::make_shared<LeftOp>(pParent, pParentRegion);
	}

	if (Check(BrainFckToken::TokenType::Move_Right))
	{
		Match(BrainFckToken::TokenType::Move_Right);
		return std::make_shared<RightOp>(pParent, pParentRegion);
	}

	if (Check(BrainFckToken::TokenType::Increment))
	{
		Match(BrainFckToken::TokenType::Increment);
		return std::make_shared<IncOp>(pParent, pParentRegion);
	}

	if (Check(BrainFckToken::TokenType::Decrement))
	{
		Match(BrainFckToken::TokenType::Decrement);
		return std::make_shared<DecOp>(pParent, pParentRegion);
	}

	if (Check(BrainFckToken::TokenType::Input_Char))
	{
		Match(BrainFckToken::TokenType::Input_Char);
		return std::make_shared<InOp>(pParent, pParentRegion);
	}

	if (Check(BrainFckToken::TokenType::Output_Char))
	{
		Match(BrainFckToken::TokenType::Output_Char);
		return std::make_shared<OutOp>(pParent, pParentRegion);
	}

	Error("Expected an operator token (<>+-.,)");
	return std::make_shared<BrainFckError>(pParent, pParentRegion);
}

TBrainFckOperationPtr BrainFckParser::ParseLoop(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion)
{
	if (Match(BrainFckToken::TokenType::Loop_Start).m_type != BrainFckToken::TokenType::Unrecognised_Token)
	{
		std::shared_ptr<Loop> pLoop = std::make_shared<Loop>(pParent, pParentRegion);
		if (!Check(BrainFckToken::TokenType::End))
		{
			pLoop->m_pRegion->AddOperation(ParseExpression(pLoop, pLoop->m_pRegion));
		}
		else
		{
			Error("Empty program can't be parsed");
			return std::make_shared<BrainFckError>(pParent, pParentRegion);
		}

		while (!Check(BrainFckToken::TokenType::End))
		{
			if (Check(BrainFckToken::TokenType::Loop_End))
			{
				Match(BrainFckToken::TokenType::Loop_End);
				break;
			}
			pLoop->m_pRegion->AddOperation(ParseExpression(pLoop, pLoop->m_pRegion));
		}
		return pLoop;
	}
	return std::make_shared<BrainFckError>(pParent, pParentRegion);
}