#pragma once

#include "BrainFckToken.h"   // for BrainFckToken

#include <ITextTokeniser.h>  // for ITextTokeniser

//////////////////////////////////////////////////////////////
class BrainFckTokeniser : public ITextTokeniser<BrainFckToken>
{
  public:
    BrainFckTokeniser() = default;

    //////////////////////////////////////////////////////////////
    virtual void ResetImplementation() override
    {
    };

  private:
    inline BrainFckToken::ETokenType::Enum CharToToken(const char chr) const;

    BrainFckToken GetNextToken();
    virtual BrainFckToken Pop_Internal() override;
};
