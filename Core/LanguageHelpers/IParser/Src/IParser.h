#pragma once

#include <IAST.h>
#include <ITokeniser.h>

#include <memory>
#include <string>
#include <vector>

template<typename TokenClass, typename LanguageOperations>
class IParser
{
  public:
    IParser(ITokeniser<TokenClass>* pTokeniser) : m_pTokeniser(pTokeniser)
    {
    }

    std::shared_ptr<IOperation<LanguageOperations>> Parse()
    {
        Reset();
        return Parse_Internal();
    }

  protected:
    virtual std::shared_ptr<IOperation<LanguageOperations>> Parse_Internal() = 0;

    bool Check(const TokenClass::TokenType::Enum expected)
    {
        return m_pTokeniser->Peek().m_type == expected;
    }

    bool Check(const std::vector<typename TokenClass::TokenType::Enum>& expected)
    {
        const std::vector<TokenClass> nextN = m_pTokeniser->Peek(expected.size());
        for (size_t tokenIdx = 0; tokenIdx < expected.size(); tokenIdx++)
        {
            if (nextN[tokenIdx].m_type != expected[tokenIdx])
            {
                return false;
            }
        }
        return true;
    }

    TokenClass Match(const TokenClass::TokenType::Enum expected)
    {
        if (Check(expected))
        {
            return m_pTokeniser->Pop();
        }
        std::string err = "Expected ";
        err = err + TokenClass::TokenType::ToString(expected) + " token not found";
        return Error(err.c_str());
    }

    TokenClass Error(const char* errorStr)
    {
        return TokenClass(TokenClass::TokenType::Unrecognised_Token);
    }

    void Reset()
    {
        m_pTokeniser->Reset();
    }

    ITokeniser<TokenClass>* m_pTokeniser{ nullptr };
};
