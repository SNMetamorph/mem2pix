/*
    Parameters manager
*/
#pragma once
#include "pixel_format.h"
#include <stdint.h>
#include <stddef.h>

class CParamsManager
{
public:
    CParamsManager();
    void ParseParameters(int argc, char *argv[]);
    const char *GetPixelFormatAlias(PixelFormat pixelFormat);
    void SetupFormatAliasList(char *stringBuffer, size_t stringSize);
    void CheckParams();

    inline void SetProcessID(uint32_t processID)   { m_iProcessID = processID; }
    inline void SetImageWidth(uint32_t width)      { m_iImageWidth = width;     }
    inline void SetImageHeight(uint32_t height)    { m_iImageHeight = height;    }
    inline void SetDataAddress(uint8_t *address)   { m_pDataAddress = address;   }
    inline void SetPixelFormat(PixelFormat format) { m_PixelFormat = format;    }

    inline uint32_t GetProcessID()      { return m_iProcessID;      }
    inline uint32_t GetImageWidth()     { return m_iImageWidth;     }
    inline uint32_t GetImageHeight()    { return m_iImageHeight;    }
    inline uint8_t *GetDataAddress()    { return m_pDataAddress;    }
    inline PixelFormat GetPixelFormat() { return m_PixelFormat;     }
    inline bool IsBorderlessMode()      { return m_isBorderlessMode; }

private:
    struct PixelFormatEntry
    {
        const char *stringAlias;
        PixelFormat pixelFormat;
    };

    uint32_t m_iImageWidth;
    uint32_t m_iImageHeight;
    int32_t  m_iProcessID;
    uint8_t *m_pDataAddress;
    PixelFormat m_PixelFormat;
    bool m_isBorderlessMode;

    const PixelFormatEntry m_FormatList[39] = {
        {"rgb888",      PixelFormat::RGB888},
        {"bgr888",      PixelFormat::BGR888},
        {"rgb24",       PixelFormat::RGB24},
        {"bgr24",       PixelFormat::BGR24},
        {"rgb332",      PixelFormat::RGB332},
        {"rgb444",      PixelFormat::RGB444},
        {"rgb555",      PixelFormat::RGB555},
        {"bgr555",      PixelFormat::BGR555},
        {"rgb565",      PixelFormat::RGB565},
        {"bgr565",      PixelFormat::BGR565},
        {"rgba32",      PixelFormat::RGBA32},
        {"argb32",      PixelFormat::ARGB32},
        {"bgra32",      PixelFormat::BGRA32},
        {"abgr32",      PixelFormat::ABGR32},
        {"bgra32",      PixelFormat::BGRA32},
        {"abgr32",      PixelFormat::ABGR32},
        {"argb4444",    PixelFormat::ARGB4444},
        {"rgba4444",    PixelFormat::RGBA4444},
        {"abgr4444",    PixelFormat::ABGR4444},
        {"bgra4444",    PixelFormat::BGRA4444},
        {"argb1555",    PixelFormat::ARGB1555},
        {"rgba5551",    PixelFormat::RGBA5551},
        {"abgr1555",    PixelFormat::ABGR1555},
        {"bgra5551",    PixelFormat::BGRA5551},
        {"rgbx8888",    PixelFormat::RGBX8888},
        {"bgrx8888",    PixelFormat::BGRX8888},
        {"argb8888",    PixelFormat::ARGB8888},
        {"rgba8888",    PixelFormat::RGBA8888},
        {"abgr8888",    PixelFormat::ABGR8888},
        {"bgra8888",    PixelFormat::BGRA8888},
        {"yv12",        PixelFormat::YV12},
        {"iyuv",        PixelFormat::IYUV},
        {"yuy2",        PixelFormat::YUY2},
        {"uyvy",        PixelFormat::UYVY},
        {"yvyu",        PixelFormat::YVYU},
        {"argb2101010", PixelFormat::ARGB2101010},
        {"nv12",        PixelFormat::NV12},
        {"nv21",        PixelFormat::NV21},
        {"grayscale8",  PixelFormat::Grayscale8},
    };
};
