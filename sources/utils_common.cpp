#include "utils.h"
#include <iostream>


bool IsDigitString(std::string &str)
{
    for (char &symbol : str)
    {
        if (!isdigit(symbol))
            return false;
    }
    return true;
}

void ReportError(const char *errorMessage)
{
    std::cout << "ERROR: " << errorMessage << std::endl;
}

void Log(const char *message)
{
    std::cout << message << std::endl;
}
