#include "BrainFckParser.h"

#include "BrainFckAST.h"    // for TBrainFckOperationPtr, TBrainFckRegionPtr, BrainFckProgram, BrainFckError
#include "BrainFckToken.h"  // for BrainFckToken

#include <IParser.h>        // for IParser
#include <ITokeniser.h>     // for ITokeniser

#include <memory>

//////////////////////////////////////////////////////////////
BrainFckParser::BrainFckParser(ITokeniser<BrainFckToken>* pTokeniser)
    : IParser(pTokeniser)
{
}

//////////////////////////////////////////////////////////////
TBrainFckOperationPtr BrainFckParser::Parse_Internal()
{
    std::shared_ptr<BrainFckProgram> pProgram = std::make_shared<BrainFckProgram>();

    if (!Check(BrainFckToken::ETokenType::End))
    {
        pProgram->m_pRegion->AddOperation(ParseExpression(pProgram, pProgram->m_pRegion));
    }
    else
    {
        pProgram->m_pRegion->AddOperation(std::make_shared<BrainFckError>(pProgram, pProgram->m_pRegion));
        Error("Empty program can't be run");
    }

    while (!Check(BrainFckToken::ETokenType::End))
    {
        pProgram->m_pRegion->AddOperation(ParseExpression(pProgram, pProgram->m_pRegion));
    }
    return pProgram;
}

//////////////////////////////////////////////////////////////
TBrainFckOperationPtr BrainFckParser::ParseExpression(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion)
{
    if (Check(BrainFckToken::ETokenType::Loop_Start))
    {
        return ParseLoop(pParent, pParentRegion);
    }
    return ParseOp(pParent, pParentRegion);
}

//////////////////////////////////////////////////////////////
TBrainFckOperationPtr BrainFckParser::ParseOp(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion)
{
    if (Check(BrainFckToken::ETokenType::Move_Left))
    {
        Match(BrainFckToken::ETokenType::Move_Left);
        return std::make_shared<LeftOp>(pParent, pParentRegion);
    }

    if (Check(BrainFckToken::ETokenType::Move_Right))
    {
        Match(BrainFckToken::ETokenType::Move_Right);
        return std::make_shared<RightOp>(pParent, pParentRegion);
    }

    if (Check(BrainFckToken::ETokenType::Increment))
    {
        Match(BrainFckToken::ETokenType::Increment);
        return std::make_shared<IncOp>(pParent, pParentRegion);
    }

    if (Check(BrainFckToken::ETokenType::Decrement))
    {
        Match(BrainFckToken::ETokenType::Decrement);
        return std::make_shared<DecOp>(pParent, pParentRegion);
    }

    if (Check(BrainFckToken::ETokenType::Input_Char))
    {
        Match(BrainFckToken::ETokenType::Input_Char);
        return std::make_shared<InOp>(pParent, pParentRegion);
    }

    if (Check(BrainFckToken::ETokenType::Output_Char))
    {
        Match(BrainFckToken::ETokenType::Output_Char);
        return std::make_shared<OutOp>(pParent, pParentRegion);
    }

    Error("Expected an operator token (<>+-.,)");
    return std::make_shared<BrainFckError>(pParent, pParentRegion);
}

//////////////////////////////////////////////////////////////
TBrainFckOperationPtr BrainFckParser::ParseLoop(TBrainFckOperationPtr pParent, TBrainFckRegionPtr pParentRegion)
{
    if (Match(BrainFckToken::ETokenType::Loop_Start).m_type != BrainFckToken::ETokenType::Unrecognised_Token)
    {
        std::shared_ptr<Loop> pLoop = std::make_shared<Loop>(pParent, pParentRegion);
        if (!Check(BrainFckToken::ETokenType::End))
        {
            pLoop->m_pRegion->AddOperation(ParseExpression(pLoop, pLoop->m_pRegion));
        }
        else
        {
            pLoop->m_pRegion->AddOperation(std::make_shared<BrainFckError>(pLoop, pLoop->m_pRegion));
            Error("Empty loop can't be run");
            return pLoop;
        }

        while (!Check(BrainFckToken::ETokenType::End))
        {
            if (Check(BrainFckToken::ETokenType::Loop_End))
            {
                Match(BrainFckToken::ETokenType::Loop_End);
                break;
            }
            pLoop->m_pRegion->AddOperation(ParseExpression(pLoop, pLoop->m_pRegion));
        }
        return pLoop;
    }
    return std::make_shared<BrainFckError>(pParent, pParentRegion);
}
