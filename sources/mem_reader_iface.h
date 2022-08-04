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
