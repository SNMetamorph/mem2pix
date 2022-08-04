#include "utils.h"
#include <iostream>

bool Utils::IsDigitString(std::string &str)
{
    for (char &symbol : str)
    {
        if (!isdigit(symbol))
            return false;
    }
    return true;
}

void Utils::ReportError(const char *errorMessage)
{
    std::cout << "ERROR: " << errorMessage << std::endl;
}

void Utils::Log(const char *message)
{
    std::cout << message << std::endl;
}
