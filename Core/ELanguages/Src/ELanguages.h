#pragma once

#include <SmartEnums.h>
#include <cstdint>

// clang-format off
#define ELANGUAGES(x) \
    x(Piet)

CreateSmartEnum(ELanguages, ELANGUAGES);

#undef ELANGUAGES
// clang-format on
