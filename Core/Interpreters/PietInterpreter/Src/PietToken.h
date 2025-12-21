#pragma once

#include <IToken.h>      // for IToken
#include <SmartEnums.h>  // for CreateSmartEnum

#include <iostream>      // for std::ostream

//////////////////////////////////////////////////////////////
class PietToken : public IToken
{
  public:
    // clang-format off
#define ETOKENTYPE(x)    \
    x(Start)             \
    x(Value)             \
    x(Push)              \
    x(Pop)               \
    x(Add)               \
    x(Subtract)          \
    x(Multiply)          \
    x(Divide)            \
    x(Modulo)            \
    x(Not)               \
    x(Greater)           \
    x(Pointer)           \
    x(Switch)            \
    x(Duplicate)         \
    x(Roll)              \
    x(Input)             \
    x(Output)            \
    x(INT)               \
    x(CHAR)              \
    x(Input_Char)        \
    x(Input_Val)         \
    x(Output_Char)       \
    x(Output_Val)        \
    x(EnterSlide)        \
    x(ExitSlide)         \
    x(End)               \
    x(Unrecognised_Token)

    CreateSmartEnum(ETokenType, ETOKENTYPE);

#undef ETOKENTYPE
    // clang-format on

    PietToken(const ETokenType::Enum type, const int value) : m_type(type), m_value(value) {};
    PietToken(const ETokenType::Enum type) : m_type(type), m_value(0) {};

    ETokenType::Enum m_type = ETokenType::Start;
    int m_value;

    //////////////////////////////////////////////////////////////
    friend std::ostream& operator<<(std::ostream& os, const PietToken& tk)
    {
        os << PietToken::ETokenType::ToString(tk.m_type);
        if (tk.m_type == ETokenType::Push)
        {
            os << " " << tk.m_value;
        }
        return os;
    }

    //////////////////////////////////////////////////////////////
    friend bool operator==(const PietToken& tk1, const PietToken& tk2)
    {
        return tk1.m_type == tk2.m_type && tk1.m_value == tk1.m_value;
    }

    //////////////////////////////////////////////////////////////
    friend bool operator!=(const PietToken& tk1, const PietToken& tk2)
    {
        return !(tk1 == tk2);
    }
};
