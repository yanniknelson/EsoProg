#pragma once

#include "ITokeniser.h"

#include <sstream>

template<typename TokenClass>
class ITextTokeniser : public ITokeniser<TokenClass>
{
  public:
    ITextTokeniser() {};

    void SetTextStream(std::stringstream& m_code)
    {
        m_pStrStream = &m_code;
        ITokeniser<TokenClass>::Reset();
        m_lineNumber = 1;
    }

    int GetLineNumber()
    {
        return m_lineNumber;
    }

  protected:
    std::stringstream* m_pStrStream{ nullptr };
    int m_lineNumber = 1;
};
