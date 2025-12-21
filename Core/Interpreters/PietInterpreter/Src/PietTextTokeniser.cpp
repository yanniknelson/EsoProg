#include "PietTextTokeniser.h"

#include "PietToken.h"  // for PietToken

#include <cctype>       // for isspace()
#include <cstdlib>      // for std::abort()
#include <string>

//////////////////////////////////////////////////////////////
void PietTextTokeniser::ResetImplementation() 
{
};

//////////////////////////////////////////////////////////////
PietToken PietTextTokeniser::Pop_Internal()
{
    PietToken token = GetNextToken();
    if (token.m_type == PietToken::ETokenType::Push)
    {
        PietToken value = Pop_Internal();
        if (value.m_type != PietToken::ETokenType::Value)
        {
            std::abort();
        }
        token.m_value = value.m_value;
    }
    return token;
}

//////////////////////////////////////////////////////////////
PietToken::ETokenType::Enum PietTextTokeniser::StringToETokenType(std::string& rString) const
{
    if (rString == "PUSH")
    {
        return PietToken::ETokenType::Push;
    }
    else if (rString == "POP")
    {
        return PietToken::ETokenType::Pop;
    }
    else if (rString == "Add")
    {
        return PietToken::ETokenType::Add;
    }
    else if (rString == "SUB")
    {
        return PietToken::ETokenType::Subtract;
    }
    else if (rString == "MUL")
    {
        return PietToken::ETokenType::Multiply;
    }
    else if (rString == "DIV")
    {
        return PietToken::ETokenType::Divide;
    }
    else if (rString == "MOD")
    {
        return PietToken::ETokenType::Modulo;
    }
    else if (rString == "NOT")
    {
        return PietToken::ETokenType::Not;
    }
    else if (rString == "GR")
    {
        return PietToken::ETokenType::Greater;
    }
    else if (rString == "PNTR")
    {
        return PietToken::ETokenType::Pointer;
    }
    else if (rString == "SWTCH")
    {
        return PietToken::ETokenType::Switch;
    }
    else if (rString == "DUP")
    {
        return PietToken::ETokenType::Duplicate;
    }
    else if (rString == "ROLL")
    {
        return PietToken::ETokenType::Roll;
    }
    else if (rString == "END")
    {
        return PietToken::ETokenType::End;
    }
    else if (rString == "OUT")
    {
        return PietToken::ETokenType::Output;
    }
    else if (rString == "IN")
    {
        return PietToken::ETokenType::Input;
    }
    else if (rString == "CHAR")
    {
        return PietToken::ETokenType::CHAR;
    }
    else if (rString == "INT")
    {
        return PietToken::ETokenType::INT;
    }
    else
    {
        return PietToken::ETokenType::Unrecognised_Token;
    }
}

//////////////////////////////////////////////////////////////
PietToken PietTextTokeniser::GetNextToken()
{
    if (!m_pStrStream->rdbuf()->in_avail())
    {
        return PietToken(PietToken::ETokenType::End);
    }
    char ch = ' ';
    std::string word;
    while (isspace(ch))
    {
        m_pStrStream->get(ch);
        if (ch == '\n')
        {
            m_lineNumber++;
        }
    }

    PietToken::ETokenType::Enum type = PietToken::ETokenType::Unrecognised_Token;
    int value = 0;

    if (isdigit(ch))
    {
        type = PietToken::ETokenType::Value;
        m_pStrStream->putback(ch);
        *m_pStrStream >> value;
    }
    else
    {
        m_pStrStream->putback(ch);
        *m_pStrStream >> word;
        type = StringToETokenType(word);

        if (type == PietToken::ETokenType::Input)
        {
            word = "";
            *m_pStrStream >> word;
            type = StringToETokenType(word);

            if (type == PietToken::ETokenType::CHAR)
            {
                type = PietToken::ETokenType::Input_Char;
            }
            else if (type == PietToken::ETokenType::INT)
            {
                type = PietToken::ETokenType::Input_Val;
            }
        }
        else if (type == PietToken::ETokenType::Output)
        {
            word = "";
            *m_pStrStream >> word;
            type = StringToETokenType(word);

            if (type == PietToken::ETokenType::CHAR)
            {
                type = PietToken::ETokenType::Output_Char;
            }
            else if (type == PietToken::ETokenType::INT)
            {
                type = PietToken::ETokenType::Output_Val;
            }
        }
    }

    return PietToken(type, value);
}
