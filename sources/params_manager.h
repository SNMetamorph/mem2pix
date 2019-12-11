/*
    Parameters manager
*/
#pragma once
#include "params.h"
#include "pixel_format.h"
#include <stdint.h>
#include <stddef.h>

class CParamsManager
{
public:
    static CParamsManager& Instance();

    void ParseParameters(int argc, char *argv[]);
    const char *GetPixelFormatAlias(pixformat_t pixelFormat);
    void SetupFormatAliasList(char *stringBuffer, size_t stringSize);

    inline void SetProcessID(uint32_t processID)   { m_AppParams.processID   = processID; }
    inline void SetImageWidth(uint32_t width)      { m_AppParams.imageWidth  = width;     }
    inline void SetImageHeight(uint32_t height)    { m_AppParams.imageHeight = height;    }
    inline void SetDataAddress(uint8_t *address)   { m_AppParams.dataAddress = address;   }
    inline void SetPixelFormat(pixformat_t format) { m_AppParams.pixelFormat = format;    }

    inline uint32_t GetProcessID()      { return m_AppParams.processID;   }
    inline uint32_t GetImageWidth()     { return m_AppParams.imageWidth;  }
    inline uint32_t GetImageHeight()    { return m_AppParams.imageHeight; }
    inline uint8_t *GetDataAddress()    { return m_AppParams.dataAddress; }
    inline pixformat_t GetPixelFormat() { return m_AppParams.pixelFormat; }
    inline bool IsBorderlessMode()      { return m_AppParams.borderlessMode; }

private:
    CParamsManager();
    CParamsManager(const CParamsManager&) = delete;
    CParamsManager& operator=(const CParamsManager&) = delete;

    void SetupFormatList();
    formatlist_t m_FormatList;
    app_params_t m_AppParams;
};

// for suitable access to singleton
CParamsManager *paramsManager();