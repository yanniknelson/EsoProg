#pragma once

#include <string>

class CPrettyPrinter
{
public:
    static std::string FormatBrainFck(const std::string& code);
    
private:
    //Checks if two characters belong to the same group
    static bool IsSameGroup(char a, char b);
    //Determines if a newline is needed between two characters
    static bool IsNewlineNeeded(char current, char next);
    //Checks if a character affects indentation
    static bool IsIndentChange(char c);
};