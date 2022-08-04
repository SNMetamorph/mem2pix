#include "utils.h"

// Windows API headers
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>


int32_t Utils::FindProcessID(const std::string &processName, size_t &processCount)
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

bool Utils::GetProcessName(int32_t processID, std::string &processName)
{
    processName.clear();
    processName.assign(256, '\0');

    HANDLE processHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION, false, processID
    );

    if (processHandle)
    {
        GetProcessImageFileName(
            processHandle, processName.data(), processName.length() - 1
        );
        CloseHandle(processHandle);
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
