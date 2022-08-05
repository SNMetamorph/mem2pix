/*
Copyright (C) 2019 SNMetamorph

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "utils.h"
#include "console.h"
#include <iostream>
#include <stdarg.h>

bool Utils::IsDigitString(std::string &str)
{
    for (char &symbol : str)
    {
        if (!std::isdigit(symbol))
            return false;
    }
    return true;
}

void Utils::ReportError(const char *errorMessage)
{
    namespace con = Console;
    namespace color = Console::TextColor;
    std::cout << color::Red << "Error: " << con::ResetTextColor << errorMessage << std::endl;
}

void Utils::Snprintf(std::string &result, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int stringSize = std::vsnprintf(nullptr, 0, format, args);
    result.assign(stringSize + 2, '\0');
    std::vsnprintf(result.data(), stringSize + 1, format, args);
    result.assign(result.data());
    va_end(args);
}
