#include "mem_reader.h"
#include <stdio.h>
#include <string.h>

IMemoryReader *g_pMemoryReader;

CMemoryReaderLinux::~CMemoryReaderLinux()
{
    CloseRemoteProcess();
}

bool CMemoryReaderLinux::OpenRemoteProcess(int32_t processID)
{
    m_iProcessID = processID;
    return true;
}

bool CMemoryReaderLinux::ReadRemoteMemory(uint8_t *address, size_t byteCount, uint8_t *buffer)
{
    size_t bytesRead = 0;
    off_t memoryOffset = reinterpret_cast<off_t>(address);

    m_iovLocal.iov_base     = buffer;
    m_iovLocal.iov_len      = byteCount;
    m_iovRemote.iov_base    = address;
    m_iovRemote.iov_len     = byteCount;

    bytesRead = process_vm_readv(
        m_iProcessID, &m_iovLocal, 1, &m_iovRemote, 1, 0
    );

    if (bytesRead == byteCount)
        return true;
    else
        return false;
}

void CMemoryReaderLinux::CloseRemoteProcess()
{
}
