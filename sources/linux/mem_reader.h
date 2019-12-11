#pragma once
#include "mem_reader_iface.h"
#include <sys/uio.h>

class CMemoryReaderLinux : public IMemoryReader
{
public:
    CMemoryReaderLinux() {};
    ~CMemoryReaderLinux();

    bool OpenRemoteProcess(int32_t processID) override;
    bool ReadRemoteMemory(
        uint8_t *address, size_t byteCount, uint8_t *buffer
    ) override;
    void CloseRemoteProcess() override;

private:
    int m_iProcessID;
    iovec m_iovLocal;
    iovec m_iovRemote;
};
