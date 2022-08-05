/*
Copyright (C) 2022 SNMetamorph

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once
#include <string>
#include <stdint.h>
#include <iostream>

namespace Console
{
    enum class Color
    {
        Black,
        Navy,
        Green,
        Teal,
        Maroon,
        Purple,
        Olive,
        Silver,
        Grey,
        Blue,
        Lime,
        Aqua,
        Red,
        Pink,
        Yellow,
        White
    };

    void SetTextColor(Color color);
    void SetBackgroundColor(Color color);
    void ResetColors(bool reset_fg = true, bool reset_bg = true);
    std::ostream &ResetColors(std::ostream &stream);
    std::ostream &ResetTextColor(std::ostream &stream);
    std::ostream &ResetBackgroundColor(std::ostream &stream);

    namespace TextColor
    {
        std::ostream &Black(std::ostream &stream);
        std::ostream &Navy(std::ostream &stream);
        std::ostream &Green(std::ostream &stream);
        std::ostream &Teal(std::ostream &stream);
        std::ostream &Maroon(std::ostream &stream);
        std::ostream &Purple(std::ostream &stream);
        std::ostream &Olive(std::ostream &stream);
        std::ostream &Silver(std::ostream &stream);
        std::ostream &Grey(std::ostream &stream);
        std::ostream &Blue(std::ostream &stream);
        std::ostream &Lime(std::ostream &stream);
        std::ostream &Aqua(std::ostream &stream);
        std::ostream &Red(std::ostream &stream);
        std::ostream &Pink(std::ostream &stream);
        std::ostream &Yellow(std::ostream &stream);
        std::ostream &White(std::ostream &stream);
    };

    namespace BackgroundColor
    {
        std::ostream &Black(std::ostream &stream);
        std::ostream &Navy(std::ostream &stream);
        std::ostream &Green(std::ostream &stream);
        std::ostream &Teal(std::ostream &stream);
        std::ostream &Maroon(std::ostream &stream);
        std::ostream &Purple(std::ostream &stream);
        std::ostream &Olive(std::ostream &stream);
        std::ostream &Silver(std::ostream &stream);
        std::ostream &Grey(std::ostream &stream);
        std::ostream &Blue(std::ostream &stream);
        std::ostream &Lime(std::ostream &stream);
        std::ostream &Aqua(std::ostream &stream);
        std::ostream &Red(std::ostream &stream);
        std::ostream &Pink(std::ostream &stream);
        std::ostream &Yellow(std::ostream &stream);
        std::ostream &White(std::ostream &stream);
    };
};
