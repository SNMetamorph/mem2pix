#include "mem_reader.h"

CMemoryReaderWin32::~CMemoryReaderWin32()
{
    CloseRemoteProcess();
}

bool CMemoryReaderWin32::OpenRemoteProcess(int32_t processID)
{
    DWORD accessFlags; 
    accessFlags = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ; 
    m_hProcess = OpenProcess(accessFlags, FALSE, processID);

    if (!m_hProcess)
        return false;
    else
        return true;
}

bool CMemoryReaderWin32::ReadRemoteMemory(uint8_t *address, size_t byteCount, uint8_t *buffer)
{
    SIZE_T bytesReaded;
    bool isReaded = ReadProcessMemory(
        m_hProcess, address, 
        buffer, byteCount, 
        &bytesReaded
    );
    return isReaded && (bytesReaded == byteCount);
}

void CMemoryReaderWin32::CloseRemoteProcess()
{
    if (m_hProcess)
        CloseHandle(m_hProcess);
}
