#pragma once

#include "BrainFckToken.h"   // for BrainFckToken

#include <ITextTokeniser.h>  // for ITextTokeniser

//////////////////////////////////////////////////////////////
class BrainFckTokeniser : public ITextTokeniser<BrainFckToken>
{
  public:
    BrainFckTokeniser() = default;

    // ITokeniser
    virtual void ResetImplementation() override;
    // ~ITokeniser

  private:
    inline BrainFckToken::ETokenType::Enum CharToToken(const char chr) const;
    BrainFckToken GetNextToken();

    // ITokeniser
    virtual BrainFckToken Pop_Internal() override;
    // ~ITokeniser
};
