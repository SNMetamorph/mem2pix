#pragma once
#include "mem_reader_iface.h"

class CMemoryReaderLinux : public IMemoryReader
{
public:
    CMemoryReaderLinux() : m_hFile(NULL) {};
    ~CMemoryReaderLinux();

    bool OpenRemoteProcess(int32_t processID) override;
    bool ReadRemoteMemory(
        uint8_t *address, size_t byteCount, uint8_t *buffer
    ) override;
    void CloseRemoteProcess() override;

private:
    int m_hFile;
    int m_iProcessID;
};