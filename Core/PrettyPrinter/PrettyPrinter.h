#pragma once

#include <string>

class CPrettyPrinter
{
public:
    static std::string FormatBrainFck(const std::string& code);
    
private:
    static bool IsSameGroup(char a, char b);
    static bool IsNewlineNeeded(char current, char next);
    static bool IsIndentChange(char c);
};