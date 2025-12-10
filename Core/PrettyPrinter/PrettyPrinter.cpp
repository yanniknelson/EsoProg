#include "PrettyPrinter.h"
#include <sstream>

namespace
{
    const std::string INDENT = "\t";
}

bool CPrettyPrinter::IsSameGroup(char a, char b)
{
    return (a == b);
}

bool CPrettyPrinter::IsNewlineNeeded(char current, char next)
{
    if ((current == '.' && next != '.') || (current == ',' && next != ','))
    {
        return true;
    }
    
    if ((current == '+' || current == '-') && 
        next != '.' && next != ',' && next != current)
    {
        return true;
    }
    
    if ((current == '>' && next == '<') || 
        (current == '<' && next == '>'))
    {
        return true;
    }
    
    return false;
}

bool CPrettyPrinter::IsIndentChange(char c)
{
    return (c == '[' || c == ']');
}

std::string CPrettyPrinter::FormatBrainFck(const std::string& code)
{
    if (code.empty())
    {
        return "";
    }
    
    std::stringstream output;
    int indentLevel = 0;
    bool newLinePending = false;
    
    for (size_t i = 0; i < code.size(); ++i)
    {
        char current = code[i];
        
        // Handle indentation changes for ']' before the character
        if (current == ']' && indentLevel > 0)
        {
            --indentLevel;
            newLinePending = true;
        }
        
        // Add newline and indentation if needed
        if (newLinePending)
        {
            output << '\n';
            for (int j = 0; j < indentLevel; ++j)
            {
                output << INDENT;
            }
            newLinePending = false;
        }
        
        output << current;
        
        // Handle indentation for '[' and newline changes
        if (current == '[')
        {
            ++indentLevel;
            newLinePending = true;
        }
        else if (current == ']')
        {
            newLinePending = true;
        }
        else if (i < code.size() - 1)
        {
            char next = code[i + 1];
            if (IsNewlineNeeded(current, next))
            {
                newLinePending = true;
            }
        }
    }
    
    return output.str();
}