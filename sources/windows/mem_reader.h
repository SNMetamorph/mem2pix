#pragma once
#include "mem_reader_iface.h"
#include <Windows.h>

class CMemoryReaderWin32 : public IMemoryReader
{
public:
    CMemoryReaderWin32() : m_hProcess(NULL) {};
    ~CMemoryReaderWin32();

    bool OpenRemoteProcess(int32_t processID) override;
    bool ReadRemoteMemory(
        uint8_t *address, size_t byteCount, uint8_t *buffer
    ) override;
    void CloseRemoteProcess() override;

private:
    HANDLE m_hProcess;
};