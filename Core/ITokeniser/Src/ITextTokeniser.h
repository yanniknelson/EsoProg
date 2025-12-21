#pragma once

#include "ITokeniser.h"

#include <iostream>
#include <strstream>

template<typename TokenClass>
class ITextTokeniser : public ITokeniser<TokenClass>
{
  public:
    ITextTokeniser() {};

    void SetTextStream(std::stringstream& m_code)
    {
        m_pStrStream = &m_code;
        m_lineNumber = 1;
    }

    int GetLineNumber()
    {
        return m_lineNumber;
    }

  protected:
    /// <summary>
    /// Convert from string to TokenType enum
    /// </summary>
    /// <param name="rString - "> string to be converted </param>
    /// <returns> TokenType enum corresponding to input string </returns>
    virtual TokenClass::TokenType StringToTokenType(std::string& rString) const
    {
        return TokenClass::TokenType::End;
    }

    std::stringstream* m_pStrStream{ nullptr };
    int m_lineNumber = 1;
};
