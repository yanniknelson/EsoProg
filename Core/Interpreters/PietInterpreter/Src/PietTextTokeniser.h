#pragma once

#include "PietToken.h"        // for PietToken

#include <ITextTokeniser.h>   // ITextTokeniser

#include <string>

//////////////////////////////////////////////////////////////
class PietTextTokeniser : public ITextTokeniser<PietToken>
{
  public:
    PietTextTokeniser() = default;

    //////////////////////////////////////////////////////////////
    virtual void ResetImplementation() override
    {
    };

  private:
    PietToken::ETokenType::Enum StringToETokenType(std::string& rString) const;

    PietToken GetNextToken();
    virtual PietToken Pop_Internal() override;
};
