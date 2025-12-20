#pragma once

#include <SmartEnums.h>
#include <cstdint>

#define ELANGUAGES(x) \
    x(Piet)           \
        x(Brainfck)

CreateSmartEnum(ELanguages, ELANGUAGES);

#undef ELANGUAGES
