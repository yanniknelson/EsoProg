#include <cstdint>   // for uint8_t
#include <iostream>  // for std::ostream
#include <string.h>  // for strcmp

#define BIT(x) (1 << (x))

#define MacroListToEnum(val) val,
#define MacroListTotoString(val) \
    case (Enum::val):            \
    {                            \
        return #val;             \
    }
#define MacroListTofromString(val) \
    if (strcmp(#val, value) == 0)  \
    {                              \
        return Enum::val;          \
    }

#define CreateSmartEnumOfType(enumName, baseType, enumValues, ...)       \
    class enumName                                                       \
    {                                                                    \
      public:                                                            \
        enum Enum : baseType                                             \
        {                                                                \
            enumValues(MacroListToEnum) COUNT                            \
        };                                                               \
        static const char* ToString(enum Enum value)                     \
        {                                                                \
            switch (value)                                               \
            {                                                            \
            enumValues(MacroListTotoString) case (Enum::COUNT):          \
            {                                                            \
                return "COUNT";                                          \
            }                                                            \
            default:                                                     \
            {                                                            \
                return "";                                               \
            }                                                            \
            }                                                            \
        };                                                               \
        static enum Enum FromString(const char* value)                   \
        {                                                                \
            enumValues(MacroListTofromString) return Enum::COUNT;        \
        };                                                               \
        friend std::ostream& operator<<(std::ostream& os, const Enum& e) \
        {                                                                \
            os << ToString(e);                                           \
            return os;                                                   \
        }                                                                \
    };

#define CreateSmartEnum(enumName, enumValues, ...) CreateSmartEnumOfType(enumName, uint8_t, enumValues, ...)
