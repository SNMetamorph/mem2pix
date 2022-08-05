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

#include "params_manager.h"
#include "exception.h"
#include "utils.h"
#include <string>
#include <string.h>
#include <stdio.h>

CParamsManager::CParamsManager()
{
    m_iImageWidth       = 0;
    m_iImageHeight      = 0;
    m_iProcessID        = -1;
    m_pDataAddress      = nullptr;
    m_PixelFormat       = PixelFormat::Invalid;
    m_isBorderlessMode  = false;
}

void CParamsManager::ParseParameters(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string parameter = argv[i];
        // params with argument
        if (i < argc - 1)
        {
            if (parameter.compare("-p") == 0)
            {
                int32_t processID;
                size_t processCount;
                std::string argument = argv[++i];
                
                if (Utils::IsDigitString(argument))
                {
                    processID = stoi(argument, nullptr, 0);
                    if (!Utils::IsValidProcessID(processID))
                        EXCEPT("invalid process ID");
                }
                else
                {
#ifdef WIN32
                    processID = Utils::FindProcessID(argument, processCount);
                    if (processCount > 1)
                    {
                        EXCEPT(
                            "found several processes with same name, "
                            "specify process ID instead name"
                        );
                    }
#else
                    EXCEPT("on Linux you should specify process ID instead name");
#endif
                }

                SetProcessID(processID);
                continue;
            }
            else if (parameter.compare("-w") == 0)
            {
                std::string argument = argv[++i];
                uint32_t width = stoi(argument, nullptr, 0);
                SetImageWidth(width);
                continue;
            }
            else if (parameter.compare("-h") == 0)
            {
                std::string argument = argv[++i];
                uint32_t height = stoi(argument, nullptr, 0);
                SetImageHeight(height);
                continue;
            }
            else if (parameter.compare("-o") == 0)
            {
                uint8_t *address;
                uint64_t offset;
                std::string argument = argv[++i];

                if (Utils::IsDigitString(argument))
                    offset = stoll(argument, nullptr, 0);
                else
                    offset = stoll(argument, nullptr, 16);

                address = reinterpret_cast<uint8_t*>(offset);
                SetDataAddress(address);
                continue;
            }
            else if (parameter.compare("-f") == 0)
            {
                std::string argument = argv[++i];
                for (const PixelFormatEntry &entry : m_FormatList)
                {
                    if (argument.compare(entry.stringAlias) == 0)
                    {
                        SetPixelFormat(entry.pixelFormat);
                        break;
                    }
                }
                continue;
            }
        }

        // params without argument
        if (parameter.compare("-b") == 0)
            m_isBorderlessMode = true;
        else
            EXCEPT("unknown startup parameter");
    }
}

void CParamsManager::CheckParams()
{
    if (m_iProcessID < 0)
        EXCEPT("invalid process name/ID, check for valid");
    else if (m_iImageWidth < 1)
        EXCEPT("invalid image width");
    else if (m_iImageHeight < 1)
        EXCEPT("invalid image height");
    else if (m_PixelFormat == PixelFormat::Invalid)
        EXCEPT("invalid pixel format, must match one from list");
}

const char *CParamsManager::GetPixelFormatAlias(PixelFormat pixelFormat)
{
    for (const PixelFormatEntry &entry : m_FormatList)
    {
        if (entry.pixelFormat == pixelFormat)
            return entry.stringAlias;
    }
    return nullptr;
}

void CParamsManager::SetupFormatAliasList(char *stringBuffer, size_t bufferSize)
{
    char lineBuffer[64];
    char linePrefix[32];
    size_t printedValues = 0;
    const size_t valuesInRow = 6;

    std::snprintf(linePrefix, sizeof(linePrefix), "%12c", ' ');
    memset(lineBuffer, 0, sizeof(lineBuffer));
    std::strncat(stringBuffer, linePrefix, bufferSize);

    for (const PixelFormatEntry &entry : m_FormatList)
    {
        snprintf(lineBuffer, sizeof(lineBuffer), "%-11s", entry.stringAlias);

        if (printedValues % valuesInRow == (valuesInRow - 1))
        {
            std::strncat(lineBuffer, "\n", sizeof(lineBuffer));
            std::strncat(lineBuffer, linePrefix, sizeof(lineBuffer));
        }

        std::strncat(stringBuffer, lineBuffer, bufferSize);
        ++printedValues;
    }
}
