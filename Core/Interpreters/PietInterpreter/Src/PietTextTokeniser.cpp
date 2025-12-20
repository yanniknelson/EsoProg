#include "PietTextTokeniser.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

PietToken PietTextTokeniser::GetNextToken()
{
    if (!m_pStrStream->rdbuf()->in_avail())
    {
        return PietToken(PietToken::TokenType::End);
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

    PietToken::TokenType::Enum type = PietToken::TokenType::Unrecognised_Token;
    int value = 0;

    if (isdigit(ch))
    {
        type = PietToken::TokenType::Value;
        m_pStrStream->putback(ch);
        *m_pStrStream >> value;
    }
    else
    {
        m_pStrStream->putback(ch);
        *m_pStrStream >> word;
        type = StringToTokenType(word);

        if (type == PietToken::TokenType::Input)
        {
            word = "";
            *m_pStrStream >> word;
            type = StringToTokenType(word);

            if (type == PietToken::TokenType::CHAR)
            {
                type = PietToken::TokenType::Input_Char;
            }
            else if (type == PietToken::TokenType::INT)
            {
                type = PietToken::TokenType::Input_Val;
            }
        }
        else if (type == PietToken::TokenType::Output)
        {
            word = "";
            *m_pStrStream >> word;
            type = StringToTokenType(word);

            if (type == PietToken::TokenType::CHAR)
            {
                type = PietToken::TokenType::Output_Char;
            }
            else if (type == PietToken::TokenType::INT)
            {
                type = PietToken::TokenType::Output_Val;
            }
        }
    }

    return PietToken(type, value);
}

PietToken PietTextTokeniser::Pop_Internal()
{
    PietToken token = GetNextToken();
    if (token.m_type == PietToken::TokenType::Push)
    {
        PietToken value = Pop_Internal();
        if (value.m_type != PietToken::TokenType::Value)
        {
            std::abort();
        }
        token.m_value = value.m_value;
    }
    return token;
}

PietToken::TokenType::Enum PietTextTokeniser::StringToTokenType(std::string& rString) const
{
    if (rString == "PUSH")
    {
        return PietToken::TokenType::Push;
    }
    else if (rString == "POP")
    {
        return PietToken::TokenType::Pop;
    }
    else if (rString == "Add")
    {
        return PietToken::TokenType::Add;
    }
    else if (rString == "SUB")
    {
        return PietToken::TokenType::Subtract;
    }
    else if (rString == "MUL")
    {
        return PietToken::TokenType::Multiply;
    }
    else if (rString == "DIV")
    {
        return PietToken::TokenType::Divide;
    }
    else if (rString == "MOD")
    {
        return PietToken::TokenType::Modulo;
    }
    else if (rString == "NOT")
    {
        return PietToken::TokenType::Not;
    }
    else if (rString == "GR")
    {
        return PietToken::TokenType::Greater;
    }
    else if (rString == "PNTR")
    {
        return PietToken::TokenType::Pointer;
    }
    else if (rString == "SWTCH")
    {
        return PietToken::TokenType::Switch;
    }
    else if (rString == "DUP")
    {
        return PietToken::TokenType::Duplicate;
    }
    else if (rString == "ROLL")
    {
        return PietToken::TokenType::Roll;
    }
    else if (rString == "END")
    {
        return PietToken::TokenType::End;
    }
    else if (rString == "OUT")
    {
        return PietToken::TokenType::Output;
    }
    else if (rString == "IN")
    {
        return PietToken::TokenType::Input;
    }
    else if (rString == "CHAR")
    {
        return PietToken::TokenType::CHAR;
    }
    else if (rString == "INT")
    {
        return PietToken::TokenType::INT;
    }
    else
    {
        return PietToken::TokenType::Unrecognised_Token;
    }
}
