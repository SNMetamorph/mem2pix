#include "core.h"
#include "utils.h"
#include "renderer.h"
#include "mem_reader.h"
#include "params_manager.h"
#include <string>

using namespace std;
IMemoryReader *g_pMemoryReader;

static bool ProgramLoop()
{
    bool     readStatus;
    size_t   bufferSize;
    uint8_t *bufferAddr;
    uint8_t *remoteAddr;

    bufferAddr = renderer()->GetFramebuffer();
    bufferSize = renderer()->GetFramebufferSize();
    remoteAddr = paramsManager()->GetDataAddress();
    readStatus = g_pMemoryReader->ReadRemoteMemory(
        remoteAddr, bufferSize, bufferAddr
    );
    
    if (readStatus)
    {
        if (!renderer()->HandleEvents())
            return false;
        renderer()->UpdateTexture();
        renderer()->DrawFrame();
    }
    else
        throw exception("failed to read remote process memory");

    return true;
}

void ProgramInit()
{ 
#ifdef WIN32
    CMemoryReaderWin32 memReader;
#else
    CMemoryReaderLinux memReader;
#endif
    int32_t processID;
    uint32_t imageWidth;
    uint32_t imageHeight;
    pixformat_t pixelFormat;
    bool isBorderless;

    g_pMemoryReader = &memReader;
    processID = paramsManager()->GetProcessID();
    imageWidth = paramsManager()->GetImageWidth();
    imageHeight = paramsManager()->GetImageHeight();
    pixelFormat = paramsManager()->GetPixelFormat();
    isBorderless = paramsManager()->IsBorderlessMode();

    Log("Initializing memory reader");
    if (!g_pMemoryReader->OpenRemoteProcess(processID))
        throw exception("memory reader initializing failed");

    Log("Initializing renderer");
    if (!renderer()->Init(imageWidth, imageHeight, pixelFormat, isBorderless))
        throw exception("renderer initializing failed");

    Log("Starting monitoring loop");
    while (ProgramLoop());
}

void ProgramParseParams(int argc, char *argv[], app_params_t &params)
{
    Log("Parsing parameters");
    paramsManager()->ParseParameters(argc, argv);

    params.processID        = paramsManager()->GetProcessID();
    params.imageWidth       = paramsManager()->GetImageWidth();
    params.imageHeight      = paramsManager()->GetImageWidth();
    params.pixelFormat      = paramsManager()->GetPixelFormat();
    params.dataAddress      = paramsManager()->GetDataAddress();
}