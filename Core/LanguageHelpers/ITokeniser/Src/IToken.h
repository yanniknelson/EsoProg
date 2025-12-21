#pragma once

#include <iostream>  // for std::ostream

//////////////////////////////////////////////////////////////
class IToken
{
  public:
    //////////////////////////////////////////////////////////////
    friend std::ostream& operator<<(std::ostream& rOS, const IToken& rTk)
    {
        rOS << "Unimplemented";
        return rOS;
    }
};
