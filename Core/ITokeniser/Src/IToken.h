#pragma once

#include <iostream>

class IToken
{
  public:
    friend std::ostream& operator<<(std::ostream& os, const IToken& tk)
    {
        os << "Unimplemented";
        return os;
    }
};
