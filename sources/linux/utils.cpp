#include "utils.h"
#include <cctype>

// Linux API headers
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int32_t FindProcessID(std::string& processName, size_t &processCount)
{
    // just a stub
    return -1;
}

bool GetProcessName(int32_t processID, char *nameBuffer, size_t bufferSize)
{
    FILE *fileHandle;
    char *lastSlash;
    char *processName;
    char stringBuffer[256];
    const size_t bufferLen = sizeof(stringBuffer);

    snprintf(
        stringBuffer, bufferLen, 
        "/proc/%d/cmdline", processID
    );
    fileHandle = fopen(stringBuffer, "rb");
    if (!fileHandle)
    {
        nameBuffer[0] = '\0';
        return false;
    }

    fread(stringBuffer, bufferLen, 1, fileHandle);
    stringBuffer[bufferLen - 1] = '\0';
    lastSlash = strrchr(stringBuffer, '/');

    if (lastSlash)
        processName = lastSlash + 1;
    else 
        processName = stringBuffer;

    if (strlen(stringBuffer) > 0)
    {
        strncpy(nameBuffer, processName, bufferSize);
        fclose(fileHandle);
        return true;
    }

    nameBuffer[0] = '\0';
    fclose(fileHandle);
    return false;
}

bool IsValidProcessID(int32_t processID)
{
    return getpgid(processID) >= 0;
}