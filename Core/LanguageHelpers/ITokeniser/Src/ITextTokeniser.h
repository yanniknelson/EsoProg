#pragma once

#include "ITokeniser.h"  // for ITokeniser

#include <sstream>       // for std::stringstream

//////////////////////////////////////////////////////////////
template<typename TokenClass>
class ITextTokeniser : public ITokeniser<TokenClass>
{
  public:
    //////////////////////////////////////////////////////////////
    ITextTokeniser() {};

    //////////////////////////////////////////////////////////////
    void SetTextStream(std::stringstream& rCode)
    {
        m_pStrStream = &rCode;
        ITokeniser<TokenClass>::Reset();
        m_lineNumber = 1;
    }

    //////////////////////////////////////////////////////////////
    int GetLineNumber()
    {
        return m_lineNumber;
    }

  protected:
    std::stringstream* m_pStrStream{ nullptr };
    int m_lineNumber = 1;
};
