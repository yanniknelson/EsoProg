#pragma once

#include <SmartEnums.h>  // for CreateSmartEnum

// clang-format off
#define ELANGUAGES(x) \
    x(Piet)           \
    x(Brainfck)

CreateSmartEnum(ELanguages, ELANGUAGES);

#undef ELANGUAGES
// clang-format on
