#pragma once

#include "BrainFckToken.h"   // for BrainFckToken

#include <ITextTokeniser.h>  // for ITextTokeniser

class BrainFckTokeniser : public ITextTokeniser<BrainFckToken>
{
  public:
    BrainFckTokeniser() {};
    virtual void ResetImplementation() override {};

  private:
    /// <summary>
    /// Convert from char to TokenType enum
    /// </summary>
    /// <param name="chr - "> char to be converted </param>
    /// <returns> TokenType enum corresponding to input char </returns>
    inline BrainFckToken::TokenType::Enum CharToToken(const char chr) const;

    BrainFckToken GetNextToken();
    virtual BrainFckToken Pop_Internal() override;
};
