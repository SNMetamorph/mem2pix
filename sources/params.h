/*
    Parameters format
*/
#pragma once
#include <stdint.h>
#include "pixel_format.h"

struct app_params_t
{
    uint32_t imageWidth;
    uint32_t imageHeight;
    int32_t  processID;
    uint8_t  *dataAddress;
    pixformat_t pixelFormat;
    bool borderlessMode;
};