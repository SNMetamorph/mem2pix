#include "application.h"
#include "utils.h"
#include "renderer.h"
#include "exception.h"
#include "mem_reader.h"
#include "params_manager.h"
#include "project_info.h"
#include <string>
#include <stdio.h>

void CApplication::UpdateWindowTitle()
{
    size_t dataOffset;
    int32_t processID;
    uint32_t imageWidth;
    uint32_t imageHeight;
    std::string processName;
    std::string titleString;
    const char *pixelFormatAlias;

    processID = m_ParamsManager.GetProcessID();
    imageWidth = m_ParamsManager.GetImageWidth();
    imageHeight = m_ParamsManager.GetImageHeight();
    dataOffset = reinterpret_cast<size_t>(
        m_ParamsManager.GetDataAddress()
    );
    pixelFormatAlias = m_ParamsManager.GetPixelFormatAlias(
        m_ParamsManager.GetPixelFormat()
    );
    Utils::GetProcessName(processID, processName);

    Utils::Snprintf(
        titleString, "%s | 0x%X | %dx%d (%s)",
        processName.c_str(), dataOffset, imageWidth, imageHeight, pixelFormatAlias
    );
    m_Renderer.UpdateWindowTitle(titleString);
}

bool CApplication::ProgramLoop()
{
    bool     readStatus;
    size_t   bufferSize;
    uint8_t *bufferAddr;
    uint8_t *remoteAddr;

    bufferAddr = m_Renderer.GetFramebuffer();
    bufferSize = m_Renderer.GetFramebufferSize();
    remoteAddr = m_ParamsManager.GetDataAddress();
    readStatus = g_pMemoryReader->ReadRemoteMemory(
        remoteAddr, bufferSize, bufferAddr
    );
    
    if (readStatus)
    {
        if (!m_Renderer.HandleEvents())
            return false;

        m_Renderer.UpdateTexture();
        m_Renderer.DrawFrame();
    }
    else
        EXCEPT("failed to read remote process memory");

    return true;
}

int CApplication::Initialize(int argc, char *argv[])
{
    if (argc <= 1)
    {
        ShowTitleText();
        return 0;
    }

    std::printf(
        "\n"
        " * \n"
        " * mem2pix started\n"
        " * \n"
        "\n"
    );

    try
    {
        ParseParams(argc, argv);
        m_ParamsManager.CheckParams();
        ReportParams();
        StartScan();
    }
    catch (const CException &ex)
    {
        Utils::ReportError(ex.GetDescription());
        return -1;
    }
    return 0;
}

void CApplication::StartScan()
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
    processID = m_ParamsManager.GetProcessID();
    imageWidth = m_ParamsManager.GetImageWidth();
    imageHeight = m_ParamsManager.GetImageHeight();
    pixelFormat = m_ParamsManager.GetPixelFormat();
    isBorderless = m_ParamsManager.IsBorderlessMode();
    UpdateWindowTitle();

    Utils::Log("Initializing memory reader");
    if (!g_pMemoryReader->OpenRemoteProcess(processID))
        EXCEPT("memory reader initializing failed");

    Utils::Log("Initializing renderer");
    m_Renderer.Init(imageWidth, imageHeight, pixelFormat, isBorderless);
    
    Utils::Log(
        "Visualizing started\n"
        "\n"
        "Usage tips:\n"
        "  Hold LMB to drag window\n"
        "  Press RMB to close window\n"
        "\n"
    );
    while (ProgramLoop());
}

void CApplication::ParseParams(int argc, char *argv[])
{
    try {
        m_ParamsManager.ParseParameters(argc, argv);
    }
    // catch STL functions exceptions
    catch (std::exception &ex) {
        EXCEPT(ex.what());
    }
}

void CApplication::ReportParams()
{
    size_t dataOffset = reinterpret_cast<size_t>(
        m_ParamsManager.GetDataAddress()
    );

    std::printf(
        "Parameters:\n"
        "  Process ID     : %d\n"
        "  Data offset    : 0x%X\n"
        "  Image size     : %dx%d\n"
        "  Pixel format   : %s\n"
        "\n",
        m_ParamsManager.GetProcessID(), dataOffset,
        m_ParamsManager.GetImageWidth(), m_ParamsManager.GetImageHeight(),
        m_ParamsManager.GetPixelFormatAlias(m_ParamsManager.GetPixelFormat())
    );
}

void CApplication::ShowTitleText()
{
    static char aliasList[2048];
    aliasList[0] = '\0';
    m_ParamsManager.SetupFormatAliasList(
        aliasList, sizeof(aliasList)
    );
    printf(
        "\n"
        "  mem2pix - utility for visualizing process memory content as image\n"
        "  Version :  %s\n"
        "  Usage   :  mem2pix <required params> [optional params]\n"
        "\n"
        "  Required parameters:\n"
        "     -p   : Target process ID or name (desirable to use ID)\n"
        "     -o   : Content address in target process (DEC or HEX value)\n"
        "     -w   : Image representation width\n"
        "     -h   : Image representation height\n"
        "     -f   : Image pixel format, must match one of list below:\n"
        "%s\n"
        "  Optional parameters:\n"
        "     -b   : Force borderless window mode"
        "\n", APP_VERSION, aliasList
    );
}
