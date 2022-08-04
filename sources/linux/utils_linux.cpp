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
#include <cctype>
#include <string>

// Linux API headers
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int32_t Utils::FindProcessID(const std::string& processName, size_t &processCount)
{
    // just a stub
    return -1;
}

bool Utils::GetProcessName(int32_t processID, std::string &processName)
{
    FILE *fileHandle;
    std::string sysFilePath;

    processName.clear();
    processName.assign(256, '\0');
    Utils::Snprintf(sysFilePath, "/proc/%d/cmdline", processID);

    fileHandle = fopen(sysFilePath.c_str(), "rb");
    if (fileHandle)
    {
        fread(processName.data(), processName.length() - 1, 1, fileHandle);
        fclose(fileHandle);
        processName.assign(processName.c_str());
        if (processName.length() > 0)
        {
            // remove path to file in string
            processName.erase(0, processName.find_last_of("/\\") + 1);
            processName.shrink_to_fit();
            return true;
        }
    }

    processName.clear();
    return false;
}

bool Utils::IsValidProcessID(int32_t processID)
{
    return getpgid(processID) >= 0;
}
