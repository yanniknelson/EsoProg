#include "PrettyPrinter.h"
#include <iostream>

void TestPrettyPrinter()
{
    // Test cases
    const char* testCases[] = {
        "++[->+<]",  // Simple loop
        "+++...---",  // Multiple same operators
        "++--",       // Alternating operators
        "><",         // Pointer movements
        ".,",         // I/O operations
        "--<-<<+[+[<+>--->->->-<<<]>]<<--.<++++++.<<-..<<.<+.>>.>>.<<<.+++.>>.>>-.<<<+." //Complex Example
    };

    for (const auto& test : testCases)
    {
        std::cout << "Original:\n" << test << "\n\n";
        std::string formatted = CPrettyPrinter::FormatBrainFck(test);
        std::cout << "Formatted:\n" << formatted << "\n";
        std::cout << "----------------------------------------\n";
    }
}

int main()
{
    TestPrettyPrinter();
    return 0;
}