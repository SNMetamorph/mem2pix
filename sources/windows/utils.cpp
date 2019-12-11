#include "utils.h"

// Windows API headers
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>


int32_t FindProcessID(std::string &processName, size_t &processCount)
{
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
}

bool GetProcessName(int32_t processID, char *nameBuffer, size_t bufferSize)
{
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
}

bool IsValidProcessID(int32_t processID)
{
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
}
