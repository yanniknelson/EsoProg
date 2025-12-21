#pragma once

#include <IToken.h>      // for IToken
#include <SmartEnums.h>  // for CreateSmartEnum

#include <iostream>      // for std::ostream

class BrainFckToken : public IToken
{
  public:
    // clang-format off
#define ETOKENTYPE(x)     \
    x(Start)              \
    x(Move_Left)          \
    x(Move_Right)         \
    x(Increment)          \
    x(Decrement)          \
    x(Output_Char)        \
    x(Input_Char)         \
    x(Loop_Start)         \
    x(Loop_End)           \
    x(End)                \
    x(Unrecognised_Token)

    CreateSmartEnum(ETokenType, ETOKENTYPE);

#undef ETOKENTYPE
    // clang-format on

    BrainFckToken(const ETokenType::Enum type) : m_type(type) {};

    ETokenType::Enum m_type = ETokenType::Start;

    /// <summary>
    /// Format and output the Token of the stack for and output stream
    /// </summary>
    /// <param name="os - "> The output stream </param>
    /// <param name="tk - "> The token to be output </param>
    /// <returns></returns>
    friend std::ostream& operator<<(std::ostream& os, const BrainFckToken& tk)
    {
        os << BrainFckToken::ETokenType::ToString(tk.m_type);
        return os;
    }
};
