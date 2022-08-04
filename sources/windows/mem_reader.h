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
