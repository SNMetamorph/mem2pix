/*
    Memory reader interface
*/
#pragma once
#include <stdint.h>
#include <stddef.h>

class IMemoryReader
{
public:
    virtual bool OpenRemoteProcess(int32_t processID) = 0;
    virtual bool ReadRemoteMemory(
        uint8_t *address, size_t byteCount, uint8_t *buffer) = 0;
    virtual void CloseRemoteProcess() = 0;
};