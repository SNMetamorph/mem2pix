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
