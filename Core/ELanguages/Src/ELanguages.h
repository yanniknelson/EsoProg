#pragma once

#include <SmartEnums.h>
#include <cstdint>

#define ELANGUAGES(x) \
    x(Piet)

CreateSmartEnum(ELanguages, ELANGUAGES);

#undef ELANGUAGES
