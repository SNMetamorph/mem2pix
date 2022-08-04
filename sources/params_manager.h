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
    const char *GetPixelFormatAlias(pixformat_t pixelFormat);
    void SetupFormatAliasList(char *stringBuffer, size_t stringSize);
    void CheckParams();

    inline void SetProcessID(uint32_t processID)   { m_iProcessID = processID; }
    inline void SetImageWidth(uint32_t width)      { m_iImageWidth = width;     }
    inline void SetImageHeight(uint32_t height)    { m_iImageHeight = height;    }
    inline void SetDataAddress(uint8_t *address)   { m_pDataAddress = address;   }
    inline void SetPixelFormat(pixformat_t format) { m_PixelFormat = format;    }

    inline uint32_t GetProcessID()      { return m_iProcessID;      }
    inline uint32_t GetImageWidth()     { return m_iImageWidth;     }
    inline uint32_t GetImageHeight()    { return m_iImageHeight;    }
    inline uint8_t *GetDataAddress()    { return m_pDataAddress;    }
    inline pixformat_t GetPixelFormat() { return m_PixelFormat;     }
    inline bool IsBorderlessMode()      { return m_isBorderlessMode; }

private:
    void SetupFormatList();
    formatlist_t m_FormatList;

    uint32_t m_iImageWidth;
    uint32_t m_iImageHeight;
    int32_t  m_iProcessID;
    uint8_t *m_pDataAddress;
    pixformat_t m_PixelFormat;
    bool m_isBorderlessMode;
};
