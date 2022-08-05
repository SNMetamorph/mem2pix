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

#include "console.h"
#include <iostream>
#ifdef WIN32
#include <io.h>
#include <windows.h>
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#include <string.h>
#else
#error This platform not seems to be supported
#endif

#ifdef WIN32
static WORD Win32_GetConsoleDefaultAttributes(HANDLE hConsole)
{
    static WORD defaultAttributes = 0;
    CONSOLE_SCREEN_BUFFER_INFO info;

    if (!defaultAttributes && GetConsoleScreenBufferInfo(hConsole, &info)) {
        defaultAttributes = info.wAttributes;
    }

    return defaultAttributes;
}

static void Win32_SetConsoleAttributes(int fg, int bg, bool reset_fg = false, bool reset_bg = false)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD defaultAttributes = Win32_GetConsoleDefaultAttributes(hConsole);

    if (reset_fg && reset_bg)
    {
        SetConsoleTextAttribute(hConsole, defaultAttributes);
        return;
    }

    if (!GetConsoleScreenBufferInfo(hConsole, &info)) {
        return;
    }

    if (reset_fg)
    {
        info.wAttributes &= ~(info.wAttributes & 0x0F);
        info.wAttributes |= defaultAttributes & 0x0F;
    }
    else if (fg != -1) 
    {
        info.wAttributes &= ~(info.wAttributes & 0x0F);
        info.wAttributes |= (WORD)fg;
    }

    if (reset_bg)
    {
        info.wAttributes &= ~(info.wAttributes & 0xF0);
        info.wAttributes |= defaultAttributes & 0xF0;
    }
    else if (bg != -1) 
    {
        info.wAttributes &= ~(info.wAttributes & 0xF0);
        info.wAttributes |= (WORD)bg;
    }

    SetConsoleTextAttribute(hConsole, info.wAttributes);
}
#endif

static bool IsStdoutTerminal()
{
#ifdef _MSC_VER
    return _isatty(_fileno(stdout)) != 0;
#else
    return isatty(fileno(stdout)) != 0;
#endif
}


void Console::SetTextColor(Color color)
{
    if (IsStdoutTerminal()) 
    {
        switch (color) 
        {
            case Color::Black:
#ifdef WIN32
                Win32_SetConsoleAttributes(0, -1);
#else
                fprintf(stdout, "%s", "\033[30m");
#endif
                break;
            case Color::Navy:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_BLUE, -1);
#else
                fprintf(stdout, "%s", "\033[34m");
#endif
                break;
            case Color::Green:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_GREEN, -1);
#else
                fprintf(stdout, "%s", "\033[32m");
#endif
                break;
            case Color::Teal:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_BLUE | FOREGROUND_GREEN, -1);
#else
                fprintf(stdout, "%s", "\033[36m");
#endif
                break;
            case Color::Maroon:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_RED, -1);
#else
                fprintf(stdout, "%s", "\033[31m");
#endif
                break;
            case Color::Purple:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_BLUE | FOREGROUND_RED, -1);
#else
                fprintf(stdout, "%s", "\033[35m");
#endif
                break;
            case Color::Olive:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_GREEN | FOREGROUND_RED, -1);
#else
                fprintf(stdout, "%s", "\033[33m");
#endif
                break;
            case Color::Silver:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, -1);
#else
                fprintf(stdout, "%s", "\033[37m");
#endif
                break;
            case Color::Grey:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_INTENSITY, -1);
#else
                fprintf(stdout, "%s", "\033[90m");
#endif
                break;
            case Color::Blue:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_BLUE | FOREGROUND_INTENSITY, -1);
#else
                fprintf(stdout, "%s", "\033[94m");
#endif
                break;
            case Color::Lime:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_GREEN | FOREGROUND_INTENSITY, -1);
#else
                fprintf(stdout, "%s", "\033[92m");
#endif
                break;
            case Color::Aqua:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, -1);
#else
                fprintf(stdout, "%s", "\033[96m");
#endif
                break;
            case Color::Red:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
#else
                fprintf(stdout, "%s", "\033[91m");
#endif
                break;
            case Color::Pink:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
#else
                fprintf(stdout, "%s", "\033[95m");
#endif
                break;
            case Color::Yellow:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
#else
                fprintf(stdout, "%s", "\033[93m");
#endif
                break;
            case Color::White:
#ifdef WIN32
                Win32_SetConsoleAttributes(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
#else
                fprintf(stdout, "%s", "\033[97m");
#endif
                break;
        }
    }
}

void Console::SetBackgroundColor(Color color)
{
    if (IsStdoutTerminal()) 
    {
        switch (color) 
        {
            case Color::Black:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, 0);
#else
                fprintf(stdout, "%s", "\033[40m");
#endif
                break;
            case Color::Navy:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_BLUE);
#else
                fprintf(stdout, "%s", "\033[44m");
#endif
                break;
            case Color::Green:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_GREEN);
#else
                fprintf(stdout, "%s", "\033[42m");
#endif
                break;
            case Color::Teal:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN);
#else
                fprintf(stdout, "%s", "\033[46m");
#endif
                break;
            case Color::Maroon:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_RED);
#else
                fprintf(stdout, "%s", "\033[41m");
#endif
                break;
            case Color::Purple:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_BLUE | BACKGROUND_RED);
#else
                fprintf(stdout, "%s", "\033[45m");
#endif
                break;
            case Color::Olive:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_GREEN | BACKGROUND_RED);
#else
                fprintf(stdout, "%s", "\033[43m");
#endif
                break;
            case Color::Silver:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
#else
                fprintf(stdout, "%s", "\033[47m");
#endif
                break;
            case Color::Grey:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_INTENSITY);
#else
                fprintf(stdout, "%s", "\033[100m");
#endif
                break;
            case Color::Blue:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
#else
                fprintf(stdout, "%s", "\033[104m");
#endif
                break;
            case Color::Lime:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
#else
                fprintf(stdout, "%s", "\033[102m");
#endif
                break;
            case Color::Aqua:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
#else
                fprintf(stdout, "%s", "\033[106m");
#endif
                break;
            case Color::Red:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_RED | BACKGROUND_INTENSITY);
#else
                fprintf(stdout, "%s", "\033[101m");
#endif
                break;
            case Color::Pink:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
#else
                fprintf(stdout, "%s", "\033[105m");
#endif
                break;
            case Color::Yellow:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
#else
                fprintf(stdout, "%s", "\033[103m");
#endif
                break;
            case Color::White:
#ifdef WIN32
                Win32_SetConsoleAttributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
#else
                fprintf(stdout, "%s", "\033[107m");
#endif
                break;
        }
    }
}

void Console::ResetColors(bool reset_fg, bool reset_bg)
{
    if (IsStdoutTerminal()) 
    {
#ifdef WIN32
        Win32_SetConsoleAttributes(-1, -1, reset_fg, reset_bg);
#else
        fprintf(stdout, "\033[00m");
#endif
    }
}

std::ostream &Console::ResetColors(std::ostream &stream)
{
    stream.flush();
    Console::ResetColors();
    return stream;
}

std::ostream &Console::ResetTextColor(std::ostream &stream)
{
    stream.flush();
    Console::ResetColors(true, false);
    return stream;
}

std::ostream &Console::ResetBackgroundColor(std::ostream &stream)
{
    stream.flush();
    Console::ResetColors(false, true);
    return stream;
}

std::ostream &Console::TextColor::Black(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Black);
    return stream;
}

std::ostream &Console::TextColor::Navy(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Navy);
    return stream;
}

std::ostream &Console::TextColor::Green(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Green);
    return stream;
}

std::ostream &Console::TextColor::Teal(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Teal);
    return stream;
}

std::ostream &Console::TextColor::Maroon(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Maroon);
    return stream;
}

std::ostream &Console::TextColor::Purple(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Purple);
    return stream;
}

std::ostream &Console::TextColor::Olive(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Olive);
    return stream;
}

std::ostream &Console::TextColor::Silver(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Silver);
    return stream;
}

std::ostream &Console::TextColor::Grey(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Grey);
    return stream;
}

std::ostream &Console::TextColor::Blue(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Blue);
    return stream;
}

std::ostream &Console::TextColor::Lime(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Lime);
    return stream;
}

std::ostream &Console::TextColor::Aqua(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Aqua);
    return stream;
}

std::ostream &Console::TextColor::Red(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Red);
    return stream;
}

std::ostream &Console::TextColor::Pink(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Pink);
    return stream;
}

std::ostream &Console::TextColor::Yellow(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::Yellow);
    return stream;
}

std::ostream &Console::TextColor::White(std::ostream &stream)
{
    stream.flush();
    Console::SetTextColor(Color::White);
    return stream;
}

std::ostream &Console::BackgroundColor::Black(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Black);
    return stream;
}

std::ostream &Console::BackgroundColor::Navy(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Navy);
    return stream;
}

std::ostream &Console::BackgroundColor::Green(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Green);
    return stream;
}

std::ostream &Console::BackgroundColor::Teal(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Teal);
    return stream;
}

std::ostream &Console::BackgroundColor::Maroon(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Maroon);
    return stream;
}

std::ostream &Console::BackgroundColor::Purple(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Purple);
    return stream;
}

std::ostream &Console::BackgroundColor::Olive(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Olive);
    return stream;
}

std::ostream &Console::BackgroundColor::Silver(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Silver);
    return stream;
}

std::ostream &Console::BackgroundColor::Grey(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Grey);
    return stream;
}

std::ostream &Console::BackgroundColor::Blue(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Blue);
    return stream;
}

std::ostream &Console::BackgroundColor::Lime(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Lime);
    return stream;
}

std::ostream &Console::BackgroundColor::Aqua(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Aqua);
    return stream;
}

std::ostream &Console::BackgroundColor::Red(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Red);
    return stream;
}

std::ostream &Console::BackgroundColor::Pink(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Pink);
    return stream;
}

std::ostream &Console::BackgroundColor::Yellow(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::Yellow);
    return stream;
}

std::ostream &Console::BackgroundColor::White(std::ostream &stream)
{
    stream.flush();
    Console::SetBackgroundColor(Color::White);
    return stream;
}
