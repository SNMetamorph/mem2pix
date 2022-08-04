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

#include "renderer.h"
#include "exception.h"
#include "utils.h"
#include <stdint.h>
#include <string.h>

static SDL_HitTestResult HitTestCallback(
    SDL_Window *window, const SDL_Point *area, void *data)
{
    uint32_t buttonMask = SDL_GetGlobalMouseState(NULL, NULL);
    if (buttonMask & SDL_BUTTON(SDL_BUTTON_LEFT))
        return SDL_HITTEST_DRAGGABLE;
    else
        return SDL_HITTEST_NORMAL;
}

CRenderer::CRenderer()
{
    m_pWindow           = nullptr;
    m_pRenderer         = nullptr;
    m_pTexture          = nullptr;
    m_pFramebuffer      = nullptr;
    m_iFramebufferSize  = 0;
    m_iRefreshRate      = 60;
    m_isFollowCursor    = false;
    m_isLegacyDragMethod = false;
    m_szWindowTitle     = "mem2pix";
}

CRenderer::~CRenderer()
{
    Dispatch();
}

void CRenderer::Dispatch()
{
    if (m_pTexture)
        SDL_DestroyTexture(m_pTexture);

    if (m_pRenderer)
        SDL_DestroyRenderer(m_pRenderer);

    if (m_pWindow)
        SDL_DestroyWindow(m_pWindow);

    if (m_pFramebuffer)
     delete[] m_pFramebuffer;

    SDL_Quit();
}

void CRenderer::Init(uint32_t width, uint32_t height, PixelFormat pixFormat, bool isBorderless)
{
    uint32_t windowFlags;
    SDL_DisplayMode displayMode;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        EXCEPT("SDL initialization failed");

    windowFlags = SDL_WINDOW_SHOWN;
    if (isBorderless || width < 90)
        windowFlags |= SDL_WINDOW_BORDERLESS;

    m_pWindow = SDL_CreateWindow(
        m_szWindowTitle.c_str(),    // window title
        SDL_WINDOWPOS_CENTERED,     // x position
        SDL_WINDOWPOS_CENTERED,     // y position
        width,                      // window width
        height,                     // window height
        windowFlags                 // flags
    );
    if (!m_pWindow)
        EXCEPT("failed to create window");

    m_pRenderer = SDL_CreateRenderer(
        m_pWindow, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!m_pRenderer)
        EXCEPT("failed to create renderer");

    m_pTexture = SDL_CreateTexture(
        m_pRenderer, GetPixelFormat(pixFormat), 
        SDL_TEXTUREACCESS_STREAMING, width, height
    );
    if (!m_pTexture)
        EXCEPT("failed to create texture");

    CalcFramebufferSize();
    if (!AllocFramebuffer())
        EXCEPT("failed to allocate framebuffer");

    if (SDL_SetWindowHitTest(m_pWindow, HitTestCallback, NULL) == -1)
    {
        m_isLegacyDragMethod = true;
        if (windowFlags & SDL_WINDOW_BORDERLESS)
        {
            Utils::Log(
                "SetWindowHitTest() failed, will be used "
                "legacy window dragging method"
            );
        }
    }

    SDL_GetCurrentDisplayMode(0, &displayMode);
    SDL_CaptureMouse(SDL_TRUE);
    m_iWindowWidth  = width;
    m_iWindowHeight = height;
    m_iRefreshRate  = displayMode.refresh_rate;
}

void CRenderer::UpdateWindowTitle(const std::string &title)
{
    m_szWindowTitle.assign(title);
    if (m_pWindow) {
        SDL_SetWindowTitle(m_pWindow, m_szWindowTitle.c_str());
    }
}

bool CRenderer::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        SDL_MouseButtonEvent &evButton = event.button;
        SDL_MouseMotionEvent &evMotion = event.motion;
        uint32_t windowFlags = SDL_GetWindowFlags(m_pWindow);
        bool windowInFocus = windowFlags & SDL_WINDOW_MOUSE_FOCUS;

        if (event.type == SDL_QUIT)
        {
            return false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (evButton.button == SDL_BUTTON_RIGHT)
            {
                if (windowInFocus)
                    return false;
            }
        }

        if (m_isLegacyDragMethod)
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (evButton.button == SDL_BUTTON_LEFT)
                {
                    if (windowInFocus && evButton.state == SDL_PRESSED)
                        m_isFollowCursor = true;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (evButton.button == SDL_BUTTON_LEFT)
                {
                    if (windowInFocus && evButton.state == SDL_RELEASED)
                        m_isFollowCursor = false;
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                if (m_isFollowCursor)
                {
                    int windowX, windowY;
                    SDL_GetGlobalMouseState(&windowX, &windowY);
                    windowX -= m_iWindowWidth / 2;
                    windowY -= m_iWindowHeight / 2;
                    SDL_SetWindowPosition(m_pWindow, windowX, windowY);
                }
            }
        }
    }
    return true;
}

void CRenderer::CalcFramebufferSize()
{
    int rowSize;
    int textureWidth; 
    int textureHeight;
    void *pixelBuffer;

    SDL_QueryTexture(
        m_pTexture, NULL, NULL, 
        &textureWidth, &textureHeight
    );
    SDL_LockTexture(m_pTexture, NULL, &pixelBuffer, &rowSize);
    SDL_UnlockTexture(m_pTexture);
    m_iFramebufferSize = rowSize * textureHeight;
}

bool CRenderer::AllocFramebuffer()
{
    m_pFramebuffer = new uint8_t[m_iFramebufferSize];
    if (m_pFramebuffer)
        return true;
    else
        return false;
}

void CRenderer::UpdateTexture()
{
    int texturePitch;
    void *textureBuffer;

    SDL_LockTexture(m_pTexture, NULL, &textureBuffer, &texturePitch);
    memcpy(textureBuffer, m_pFramebuffer, m_iFramebufferSize);
    SDL_UnlockTexture(m_pTexture);
}

void CRenderer::DrawFrame()
{
    SDL_RenderClear(m_pRenderer);
    SDL_RenderCopy(m_pRenderer, m_pTexture, nullptr, nullptr);
    SDL_RenderPresent(m_pRenderer);
    SDL_Delay(1000 / m_iRefreshRate);
}

SDL_PixelFormatEnum CRenderer::GetPixelFormat(PixelFormat pixFormat)
{
    switch (pixFormat)
    {
        case PixelFormat::Grayscale8:
            return SDL_PIXELFORMAT_INDEX8;
        case PixelFormat::RGB332:
            return SDL_PIXELFORMAT_RGB332;
        case PixelFormat::RGB444:
            return SDL_PIXELFORMAT_RGB444;
        case PixelFormat::RGB555:
            return SDL_PIXELFORMAT_RGB555;
        case PixelFormat::BGR555:
            return SDL_PIXELFORMAT_BGR555;
        case PixelFormat::ARGB4444:
            return SDL_PIXELFORMAT_ARGB4444;
        case PixelFormat::RGBA4444:
            return SDL_PIXELFORMAT_RGBA4444;
        case PixelFormat::ABGR4444:
            return SDL_PIXELFORMAT_ABGR4444;
        case PixelFormat::BGRA4444:
            return SDL_PIXELFORMAT_BGRA4444;
        case PixelFormat::ARGB1555:
            return SDL_PIXELFORMAT_ARGB1555;
        case PixelFormat::RGBA5551:
            return SDL_PIXELFORMAT_RGBA5551;
        case PixelFormat::ABGR1555:
            return SDL_PIXELFORMAT_ABGR1555;
        case PixelFormat::BGRA5551:
            return SDL_PIXELFORMAT_BGRA5551;
        case PixelFormat::RGB565:
            return SDL_PIXELFORMAT_RGB565;
        case PixelFormat::BGR565:
            return SDL_PIXELFORMAT_BGR565;
        case PixelFormat::RGB24:
            return SDL_PIXELFORMAT_RGB24;
        case PixelFormat::BGR24:
            return SDL_PIXELFORMAT_BGR24;
        case PixelFormat::RGB888:
            return SDL_PIXELFORMAT_RGB888;
        case PixelFormat::RGBX8888:
            return SDL_PIXELFORMAT_RGBX8888;
        case PixelFormat::BGR888:
            return SDL_PIXELFORMAT_BGR888;
        case PixelFormat::BGRX8888:
            return SDL_PIXELFORMAT_BGRX8888;
        case PixelFormat::ARGB8888:
            return SDL_PIXELFORMAT_ARGB8888;
        case PixelFormat::RGBA8888:
            return SDL_PIXELFORMAT_RGBA8888;
        case PixelFormat::ABGR8888:
            return SDL_PIXELFORMAT_ABGR8888;
        case PixelFormat::BGRA8888:
            return SDL_PIXELFORMAT_BGRA8888;
        case PixelFormat::ARGB2101010:
            return SDL_PIXELFORMAT_ARGB2101010;
        case PixelFormat::RGBA32:
            return SDL_PIXELFORMAT_RGBA32;
        case PixelFormat::ARGB32:
            return SDL_PIXELFORMAT_ARGB32;
        case PixelFormat::BGRA32:
            return SDL_PIXELFORMAT_BGRA32;
        case PixelFormat::ABGR32:
            return SDL_PIXELFORMAT_ABGR32;
        case PixelFormat::YV12:
            return SDL_PIXELFORMAT_YV12;
        case PixelFormat::IYUV:
            return SDL_PIXELFORMAT_IYUV;
        case PixelFormat::YUY2:
            return SDL_PIXELFORMAT_UYVY;
        case PixelFormat::UYVY:
            return SDL_PIXELFORMAT_UYVY;
        case PixelFormat::YVYU:
            return SDL_PIXELFORMAT_YVYU;
        case PixelFormat::NV12:
            return SDL_PIXELFORMAT_NV12;
        case PixelFormat::NV21:
            return SDL_PIXELFORMAT_NV21;
        default:
            return SDL_PIXELFORMAT_UNKNOWN;
    }
}
