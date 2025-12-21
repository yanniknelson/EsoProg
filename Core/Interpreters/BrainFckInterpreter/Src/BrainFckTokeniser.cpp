#include "BrainFckTokeniser.h"

#include "BrainFckToken.h"  // for BrainFckToken

#include <cctype>           // for isspace
#include <string>

//////////////////////////////////////////////////////////////
void BrainFckTokeniser::ResetImplementation()
{
}

//////////////////////////////////////////////////////////////
inline BrainFckToken::ETokenType::Enum BrainFckTokeniser::CharToToken(const char chr) const
{
    switch (chr)
    {
    case ('>'):
    {
        return BrainFckToken::ETokenType::Move_Right;
    }
    case ('<'):
    {
        return BrainFckToken::ETokenType::Move_Left;
    }
    case ('+'):
    {
        return BrainFckToken::ETokenType::Increment;
    }
    case ('-'):
    {
        return BrainFckToken::ETokenType::Decrement;
    }
    case ('.'):
    {
        return BrainFckToken::ETokenType::Output_Char;
    }
    case (','):
    {
        return BrainFckToken::ETokenType::Input_Char;
    }
    case ('['):
    {
        return BrainFckToken::ETokenType::Loop_Start;
    }
    case (']'):
    {
        return BrainFckToken::ETokenType::Loop_End;
    }
    default:
    {
        return BrainFckToken::ETokenType::Unrecognised_Token;
    }
    }
}

//////////////////////////////////////////////////////////////
BrainFckToken BrainFckTokeniser::GetNextToken()
{
    char ch = ' ';
    BrainFckToken::ETokenType::Enum currentETokenType = BrainFckToken::ETokenType::Unrecognised_Token;
    std::string word;
    // ignore all white space and characters that aren't ><+-.,[]
    while (m_pStrStream->rdbuf()->in_avail() && (isspace(ch) || currentETokenType == BrainFckToken::ETokenType::Unrecognised_Token))
    {
        m_pStrStream->get(ch);
        currentETokenType = CharToToken(ch);
        if (ch == '\n')
        {
            m_lineNumber++;
        }
    }

    if (currentETokenType == BrainFckToken::ETokenType::Unrecognised_Token)
    {
        currentETokenType = BrainFckToken::ETokenType::End;
    }

    return currentETokenType;
}

//////////////////////////////////////////////////////////////
BrainFckToken BrainFckTokeniser::Pop_Internal()
{
    BrainFckToken token = GetNextToken();
    return token;
}
