#include "params_manager.h"
#include "exception.h"
#include "utils.h"
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;

CParamsManager *paramsManager()
{
    return &CParamsManager::Instance();
}

CParamsManager& CParamsManager::Instance()
{
    static CParamsManager instance;
    return instance;
}

CParamsManager::CParamsManager()
{
    m_AppParams.imageWidth    = 0;
    m_AppParams.imageHeight   = 0;
    m_AppParams.processID     = -1;
    m_AppParams.dataAddress   = nullptr;
    m_AppParams.pixelFormat   = PIXFORMAT_INVALID;
    m_AppParams.borderlessMode = false;
    FillFormatList();
}

void CParamsManager::FillFormatList()
{
    m_FormatList = {
        {"rgb888",      PIXFORMAT_RGB888},
        {"bgr888",      PIXFORMAT_BGR888},
        {"rgb24",       PIXFORMAT_RGB24},
        {"bgr24",       PIXFORMAT_BGR24},
        {"rgb332",      PIXFORMAT_RGB332},
        {"rgb444",      PIXFORMAT_RGB444},
        {"rgb555",      PIXFORMAT_RGB555},
        {"bgr555",      PIXFORMAT_BGR555},
        {"rgb565",      PIXFORMAT_RGB565},
        {"bgr565",      PIXFORMAT_BGR565},
        {"rgba32",      PIXFORMAT_RGBA32},
        {"argb32",      PIXFORMAT_ARGB32},
        {"bgra32",      PIXFORMAT_BGRA32},
        {"abgr32",      PIXFORMAT_ABGR32},
        {"bgra32",      PIXFORMAT_BGRA32},
        {"abgr32",      PIXFORMAT_ABGR32},
        {"argb4444",    PIXFORMAT_ARGB4444},
        {"rgba4444",    PIXFORMAT_RGBA4444},
        {"abgr4444",    PIXFORMAT_ABGR4444},
        {"bgra4444",    PIXFORMAT_BGRA4444},
        {"argb1555",    PIXFORMAT_ARGB1555},
        {"rgba5551",    PIXFORMAT_RGBA5551},
        {"abgr1555",    PIXFORMAT_ABGR1555},
        {"bgra5551",    PIXFORMAT_BGRA5551},
        {"rgbx8888",    PIXFORMAT_RGBX8888},  
        {"bgrx8888",    PIXFORMAT_BGRX8888},
        {"argb8888",    PIXFORMAT_ARGB8888},
        {"rgba8888",    PIXFORMAT_RGBA8888},
        {"abgr8888",    PIXFORMAT_ABGR8888},
        {"bgra8888",    PIXFORMAT_BGRA8888},
        {"yv12",        PIXFORMAT_YV12},
        {"iyuv",        PIXFORMAT_IYUV},
        {"yuy2",        PIXFORMAT_YUY2},
        {"uyvy",        PIXFORMAT_UYVY},
        {"yvyu",        PIXFORMAT_YVYU},
        {"argb2101010", PIXFORMAT_ARGB2101010},
        {"nv12",        PIXFORMAT_NV12},
        {"nv21",        PIXFORMAT_NV21},
        {"grayscale8",  PIXFORMAT_GRAYSCALE8},
    };
}

void CParamsManager::ParseParameters(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        string parameter = argv[i];
        // params with argument
        if (i < argc - 1)
        {
            if (parameter.compare("-p") == 0)
            {
                int32_t processID;
                size_t processCount;
                string argument = argv[++i];
                
                if (IsDigitString(argument))
                {
                    processID = stoi(argument, nullptr, 0);
                    if (!IsValidProcessID(processID))
                        EXCEPT("invalid process ID");
                }
                else
                {
#ifdef WIN32
                    processID = FindProcessID(argument, processCount);
                    if (processCount > 1)
                    {
                        EXCEPT(
                            "found several processes with same name, "
                            "specify process ID instead name"
                        );
                    }
#else
                    EXCEPT("on Linux you should specify process ID instead name");
#endif
                }

                SetProcessID(processID);
                continue;
            }
            else if (parameter.compare("-w") == 0)
            {
                string argument = argv[++i];
                uint32_t width = stoi(argument, nullptr, 0);
                SetImageWidth(width);
                continue;
            }
            else if (parameter.compare("-h") == 0)
            {
                string argument = argv[++i];
                uint32_t height = stoi(argument, nullptr, 0);
                SetImageHeight(height);
                continue;
            }
            else if (parameter.compare("-o") == 0)
            {
                uint8_t *address;
                uint64_t offset;
                string argument = argv[++i];

                if (IsDigitString(argument))
                    offset = stoll(argument, nullptr, 0);
                else
                    offset = stoll(argument, nullptr, 16);

                address = reinterpret_cast<uint8_t*>(offset);
                SetDataAddress(address);
                continue;
            }
            else if (parameter.compare("-f") == 0)
            {
                string argument = argv[++i];
                for (pixformat_entry_t &entry : m_FormatList)
                {
                    if (argument.compare(entry.stringAlias) == 0)
                    {
                        SetPixelFormat(entry.pixelFormat);
                        break;
                    }
                }
                continue;
            }
        }

        // params without argument
        if (parameter.compare("-b") == 0)
            m_AppParams.borderlessMode = true;
    }
}

const char *CParamsManager::GetPixelFormatAlias(pixformat_t pixelFormat)
{
    for (pixformat_entry_t &entry : m_FormatList)
    {
        if (entry.pixelFormat == pixelFormat)
            return entry.stringAlias;
    }
    return nullptr;
}

void CParamsManager::GetFormatAliasList(char *stringBuffer, size_t bufferSize)
{
    char lineBuffer[64];
    char linePrefix[32];
    size_t printedValues = 0;
    const size_t valuesInRow = 6;

    snprintf(linePrefix, sizeof(linePrefix), "%12c", ' ');
    memset(lineBuffer, 0, sizeof(lineBuffer));
    strncat(stringBuffer, linePrefix, bufferSize);

    for (pixformat_entry_t &entry : m_FormatList)
    {
        snprintf(lineBuffer, sizeof(lineBuffer), "%-11s", entry.stringAlias);

        if (printedValues % valuesInRow == (valuesInRow - 1))
        {
            strncat(lineBuffer, "\n", sizeof(lineBuffer));
            strncat(lineBuffer, linePrefix, sizeof(lineBuffer));
        }

        strncat(stringBuffer, lineBuffer, bufferSize);
        ++printedValues;
    }
}

