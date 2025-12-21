#pragma once

#include "PietToken.h"        // for PietToken

#include <ITextTokeniser.h>   // ITextTokeniser

#include <string>

//////////////////////////////////////////////////////////////
class PietTextTokeniser : public ITextTokeniser<PietToken>
{
  public:
    PietTextTokeniser() = default;

    // ITokeniser
    virtual void ResetImplementation() override;
    // ~ITokeniser

  private:

    // ITokeniser
    virtual PietToken Pop_Internal() override;
    // ~ITokeniser

    PietToken::ETokenType::Enum StringToETokenType(std::string& rString) const;
    PietToken GetNextToken();
};
