#include "utils.h"
#include <cctype>
#include <iostream>

#ifdef WIN32
// Windows API headers
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#endif

using namespace std;

bool IsDigitString(std::string& str)
{
    for (char &symbol : str)
    {
        if (!isdigit(symbol))
            return false;
    }
    return true;
}

int32_t FindProcessID(std::string& processName, size_t &processCount)
{
#ifdef WIN32
    int32_t processID;
    HANDLE processSnapshot;
    PROCESSENTRY32 processEntry;

    processID = -1;
    processCount = 0;
    processEntry.dwSize = sizeof(processEntry);
    processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processSnapshot == INVALID_HANDLE_VALUE)
        return -1;

    if (!Process32First(processSnapshot, &processEntry)) 
    {
        CloseHandle(processSnapshot);
        return -1;
    }

    do 
    {
        if (processName.compare(processEntry.szExeFile) == 0)
        {
            processID = processEntry.th32ProcessID;
            ++processCount;
        }
    } 
    while (Process32Next(processSnapshot, &processEntry));

    CloseHandle(processSnapshot);
    return processID;
#else
    return -1;
#endif
}

bool GetProcessName(int32_t processID, char *nameBuffer, size_t bufferSize)
{
#ifdef WIN32
    char *slashPos;
    char *processName;
    char stringBuffer[256];
    const size_t bufferLen = sizeof(stringBuffer);
    HANDLE processHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION, false, processID
    );

    if (processHandle)
    {
        stringBuffer[0] = '\0';
        GetProcessImageFileName(
            processHandle, stringBuffer, bufferLen
        );
        CloseHandle(processHandle);
    }
    else
    {
        nameBuffer[0] = '\0';
        return false;
    }

    slashPos = strrchr(stringBuffer, '\\');
    if (slashPos)
        processName = slashPos + 1;
    else
        processName = stringBuffer;

    if (strlen(stringBuffer) > 0)
    {
        strncpy(nameBuffer, processName, bufferSize);
        return true;
    }

    return false;
#else
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
#endif
}

bool IsValidProcessID(int32_t processID)
{
#ifdef WIN32
    HANDLE processSnapshot;
    PROCESSENTRY32 processEntry;

    processEntry.dwSize = sizeof(processEntry);
    processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processSnapshot == INVALID_HANDLE_VALUE)
        return false;

    if (!Process32First(processSnapshot, &processEntry)) 
    {
        CloseHandle(processSnapshot);
        return false;
    }

    do 
    {
        if (processEntry.th32ProcessID == processID)
        {
            CloseHandle(processSnapshot);
            return true;
        }
    } 
    while (Process32Next(processSnapshot, &processEntry));

    CloseHandle(processSnapshot);
    return false;
#else
    return getpgid(processID) >= 0;
#endif
}

void ReportError(const char *errorMessage)
{
    cout << "ERROR: " << errorMessage << endl;
}

void Log(const char *message)
{
    cout << message << endl;
}
