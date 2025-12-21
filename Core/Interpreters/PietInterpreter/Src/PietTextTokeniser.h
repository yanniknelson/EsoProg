#pragma once

#include "PietToken.h"        // for PietToken

#include <ITextTokeniser.h>   // ITextTokeniser

#include <string>

class PietTextTokeniser : public ITextTokeniser<PietToken>
{
  public:
    PietTextTokeniser() {};
    virtual void ResetImplementation() override {};

  private:
    /// <summary>
    /// Convert from string to ETokenType enum
    /// </summary>
    /// <param name="rString - "> string to be converted </param>
    /// <returns> ETokenType enum corresponding to input string </returns>
    PietToken::ETokenType::Enum StringToETokenType(std::string& rString) const;

    PietToken GetNextToken();
    virtual PietToken Pop_Internal() override;
};
