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
