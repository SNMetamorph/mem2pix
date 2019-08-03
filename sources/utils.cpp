#include "utils.h"
#include <cctype>
#include <iostream>

#ifdef WIN32
// Windows API headers
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
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

int32_t FindProcessID(std::string& processName)
{
#ifdef WIN32
    HANDLE processSnapshot;
    PROCESSENTRY32 processEntry;

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
            CloseHandle(processSnapshot);
            return processEntry.th32ProcessID;
        }
    } 
    while (Process32Next(processSnapshot, &processEntry));

    CloseHandle(processSnapshot);
    return -1;
#else
#error FindProcessID() not yet implemented for Linux
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
