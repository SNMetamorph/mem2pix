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

#include "application.h"
#include "utils.h"
#include "renderer.h"
#include "exception.h"
#include "params_manager.h"
#include "project_info.h"
#include "console.h"
#include <string>
#include <stdio.h>
#include <iostream>

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
    readStatus = m_pMemoryReader->ReadRemoteMemory(
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

    std::cout << std::endl;
    std::cout << " * " << std::endl;
    std::cout << " * " << Console::TextColor::Lime << "mem2pix" << Console::ResetTextColor << " started" << std::endl;
    std::cout << " * " << std::endl;
    std::cout << std::endl;

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
    int32_t processID;
    uint32_t imageWidth;
    uint32_t imageHeight;
    PixelFormat pixelFormat;
    bool isBorderless;

#ifdef WIN32
    m_pMemoryReader = std::make_unique<CMemoryReaderWin32>();
#else
    m_pMemoryReader = std::make_unique<CMemoryReaderLinux>();
#endif

    processID = m_ParamsManager.GetProcessID();
    imageWidth = m_ParamsManager.GetImageWidth();
    imageHeight = m_ParamsManager.GetImageHeight();
    pixelFormat = m_ParamsManager.GetPixelFormat();
    isBorderless = m_ParamsManager.IsBorderlessMode();
    UpdateWindowTitle();

    std::cout << "Initializing memory reader" << std::endl;
    if (!m_pMemoryReader->OpenRemoteProcess(processID))
        EXCEPT("memory reader initializing failed");

    std::cout << "Initializing renderer" << std::endl;
    m_Renderer.Init(imageWidth, imageHeight, pixelFormat, isBorderless);
    
    std::cout << "Visualizing started" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage tips:" << std::endl;
    std::cout << "  Hold LMB to drag window" << std::endl;
    std::cout << "  Press RMB to close window" << std::endl;
    std::cout << std::endl;

    while (ProgramLoop());
    std::cout << "Scanning stopped, exiting..." << std::endl;
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
    namespace con = Console;
    namespace tc = Console::TextColor;
    size_t imageWidth = m_ParamsManager.GetImageWidth();
    size_t imageHeight = m_ParamsManager.GetImageHeight();
    PixelFormat pixFormat = m_ParamsManager.GetPixelFormat();
    size_t dataOffset = reinterpret_cast<size_t>(
        m_ParamsManager.GetDataAddress()
    );
    
    std::cout << "Parameters:" << std::endl;
    std::cout << "  Process ID     : " << tc::Aqua << m_ParamsManager.GetProcessID() << std::endl << con::ResetTextColor;
    std::cout << "  Data offset    : " << tc::Aqua << "0x" << std::uppercase << std::hex << dataOffset << std::endl << std::dec << con::ResetTextColor;
    std::cout << "  Image size     : " << tc::Aqua << imageWidth << "x" << imageHeight << std::endl << con::ResetTextColor;
    std::cout << "  Pixel format   : " << tc::Aqua << m_ParamsManager.GetPixelFormatAlias(pixFormat) << std::endl << con::ResetTextColor;
    std::cout << std::endl;
}

void CApplication::ShowTitleText()
{
    namespace con = Console;
    namespace tc = Console::TextColor;
    static char aliasList[2048];
    aliasList[0] = '\0';
    m_ParamsManager.SetupFormatAliasList(aliasList, sizeof(aliasList));

    std::cout << std::endl;
    std::cout << tc::Lime << "  mem2pix" << con::ResetTextColor << " - utility for visualizing process memory content as image" << std::endl;
    std::cout << "  Version :  " << APP_VERSION_STRING << " (" << APP_BUILD_DATE << ")" << std::endl;
    std::cout << "  Usage   :  " << "mem2pix <required params> [optional params]" << std::endl;
    std::cout << std::endl;
    std::cout << "  Required parameters:" << std::endl;
    std::cout << tc::Red << "     -p" << con::ResetTextColor << "   : Target process ID or name (desirable to use ID)" << std::endl;
    std::cout << tc::Red << "     -o" << con::ResetTextColor << "   : Content address in target process (DEC or HEX value)" << std::endl;
    std::cout << tc::Red << "     -w" << con::ResetTextColor << "   : Image representation width" << std::endl;
    std::cout << tc::Red << "     -h" << con::ResetTextColor << "   : Image representation height" << std::endl;
    std::cout << tc::Red << "     -f" << con::ResetTextColor << "   : Image pixel format, must match one of list below:" << std::endl;

    con::SetTextColor(Console::Color::Olive);
    std::cout << aliasList << std::endl;
    con::ResetColors(true, false);

    std::cout << "  Optional parameters:" << std::endl;
    std::cout << tc::Red << "     -b" << con::ResetTextColor << "   : Force borderless window mode" << std::endl;
}
