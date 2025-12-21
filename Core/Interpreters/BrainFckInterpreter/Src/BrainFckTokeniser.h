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
    /// Convert from char to ETokenType enum
    /// </summary>
    /// <param name="chr - "> char to be converted </param>
    /// <returns> ETokenType enum corresponding to input char </returns>
    inline BrainFckToken::ETokenType::Enum CharToToken(const char chr) const;

    BrainFckToken GetNextToken();
    virtual BrainFckToken Pop_Internal() override;
};
