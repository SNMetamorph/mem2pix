#include "mem_reader.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

CMemoryReaderLinux::~CMemoryReaderLinux()
{
    CloseRemoteProcess();
}

bool CMemoryReaderLinux::OpenRemoteProcess(int32_t processID)
{
    char filePath[64];
    snprintf(filePath, sizeof(filePath), "/proc/%d/mem", processID);
    m_hFile = open(filePath, O_RDONLY);
    m_iProcessID = processID;

    if (m_hFile > 0)
        return true;
    else 
        return false;
}

bool CMemoryReaderLinux::ReadRemoteMemory(uint8_t *address, size_t byteCount, uint8_t *buffer)
{
    size_t bytesRead = 0;
    ptrace(PTRACE_ATTACH, m_iProcessID, 0, 0);
    waitpid(m_iProcessID, NULL, 0);
    bytesRead = pread(
        m_hFile, buffer, byteCount, reinterpret_cast<off_t>(address)
    );
    ptrace(PTRACE_DETACH, m_iProcessID, 0, 0);

    if (bytesRead == byteCount)
        return true;
    else
        return false;
}

void CMemoryReaderLinux::CloseRemoteProcess()
{
    close(m_hFile);
}
