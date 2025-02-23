#define BIT(x) (1 << (x)) 

#define MacroListToEnum(val) val,
#define MacroListTotoString(val)\
    case (Enum::val):\
    {\
        return #val;\
    }
#define MacroListTofromString(val)\
    if (strcmp(#val, value) == 0)\
    {\
        return Enum::val;\
    }

#define CreateSmartEnum(enumName, enumValues, ...)\
    class enumName\
    {\
    public:\
        enum Enum : uint8_t\
        {\
          enumValues(MacroListToEnum) COUNT\
        };\
        static const char* ToString(enum Enum value)\
        {\
            switch (value)\
            {\
                enumValues(MacroListTotoString)\
                case (Enum::COUNT):\
                {\
                    return "COUNT";\
                }\
                    default:\
                {\
                    return "";\
                }\
            }\
        };\
        static enum Enum FromString(const char* value)\
        {\
            enumValues(MacroListTofromString)\
            return Enum::COUNT;\
        };\
    }