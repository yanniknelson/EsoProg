#pragma once

#include <cstdint>
#include <SmartEnums.h>

#define ELANGUAGES(x)\
    x(Piet)

CreateSmartEnum(ELanguages, ELANGUAGES);

#undef ELANGUAGES