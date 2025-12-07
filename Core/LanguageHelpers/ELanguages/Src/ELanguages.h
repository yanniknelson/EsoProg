#pragma once

#include <cstdint>
#include <SmartEnums.h>

#define ELANGUAGES(x)\
    x(Piet)\
    x(Brainfck)\

CreateSmartEnum(ELanguages, ELANGUAGES);

#undef ELANGUAGES