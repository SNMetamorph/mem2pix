#pragma once
#include "renderer.h"
#include <stdint.h>

CRenderer& CRenderer::Instance()
{
    static CRenderer instance;
    return instance;
}

CRenderer *renderer()
{
    return &CRenderer::Instance();
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

bool CRenderer::Init(uint32_t width, uint32_t height, pixformat_t pixFormat, bool isBorderless)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return false;

    uint32_t windowFlags = SDL_WINDOW_SHOWN;
    if (isBorderless || width < 90)
        windowFlags |= SDL_WINDOW_BORDERLESS;

    m_pWindow = SDL_CreateWindow(
        "mem2pix",              // window title
        SDL_WINDOWPOS_CENTERED, // x position
        SDL_WINDOWPOS_CENTERED, // y position
        width,                  // window width
        height,                 // window height
        windowFlags             // flags
    );
    if (!m_pWindow)
        return false;

    m_pRenderer = SDL_CreateRenderer(
        m_pWindow, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!m_pRenderer)
        return false;

    m_pTexture = SDL_CreateTexture(
        m_pRenderer, GetPixelFormat(pixFormat), 
        SDL_TEXTUREACCESS_STREAMING, width, height
    );
    if (!m_pTexture)
        return false;

    FillFramebufferSize();
    if (!AllocFramebuffer())
        return false;

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    SDL_CaptureMouse(SDL_TRUE);
    m_iWindowWidth  = width;
    m_iWindowHeight = height;
    m_iRefreshRate  = displayMode.refresh_rate;

    return true;
}

bool CRenderer::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    { 
        SDL_MouseButtonEvent &evButton = event.button;
        SDL_MouseMotionEvent &evMotion = event.motion;

        if (event.type == SDL_QUIT)
            return false;
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (evButton.button == SDL_BUTTON_RIGHT)
            {
                uint32_t windowFlags = SDL_GetWindowFlags(m_pWindow);
                if (windowFlags & SDL_WINDOW_MOUSE_FOCUS)
                    return false;
            }

            if (evButton.button == SDL_BUTTON_LEFT)
            {
                if (evButton.state == SDL_PRESSED)
                    m_isFollowCursor = true;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (evButton.button == SDL_BUTTON_LEFT)
            {
                if (evButton.state == SDL_RELEASED)
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
    return true;
}

void CRenderer::FillFramebufferSize()
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

SDL_PixelFormatEnum CRenderer::GetPixelFormat(pixformat_t pixFormat)
{
    switch (pixFormat)
    {
        case PIXFORMAT_GRAYSCALE8:
            return SDL_PIXELFORMAT_INDEX8;
        case PIXFORMAT_RGB332:
            return SDL_PIXELFORMAT_RGB332;
        case PIXFORMAT_RGB444:
            return SDL_PIXELFORMAT_RGB444;
        case PIXFORMAT_RGB555:
            return SDL_PIXELFORMAT_RGB555;
        case PIXFORMAT_BGR555:
            return SDL_PIXELFORMAT_BGR555;
        case PIXFORMAT_ARGB4444:
            return SDL_PIXELFORMAT_ARGB4444;
        case PIXFORMAT_RGBA4444:
            return SDL_PIXELFORMAT_RGBA4444;
        case PIXFORMAT_ABGR4444:
            return SDL_PIXELFORMAT_ABGR4444;
        case PIXFORMAT_BGRA4444:
            return SDL_PIXELFORMAT_BGRA4444;
        case PIXFORMAT_ARGB1555:
            return SDL_PIXELFORMAT_ARGB1555;
        case PIXFORMAT_RGBA5551:
            return SDL_PIXELFORMAT_RGBA5551;
        case PIXFORMAT_ABGR1555:
            return SDL_PIXELFORMAT_ABGR1555;
        case PIXFORMAT_BGRA5551:
            return SDL_PIXELFORMAT_BGRA5551;
        case PIXFORMAT_RGB565:
            return SDL_PIXELFORMAT_RGB565;
        case PIXFORMAT_BGR565:
            return SDL_PIXELFORMAT_BGR565;
        case PIXFORMAT_RGB24:
            return SDL_PIXELFORMAT_RGB24;
        case PIXFORMAT_BGR24:
            return SDL_PIXELFORMAT_BGR24;
        case PIXFORMAT_RGB888:
            return SDL_PIXELFORMAT_RGB888;
        case PIXFORMAT_RGBX8888:
            return SDL_PIXELFORMAT_RGBX8888;
        case PIXFORMAT_BGR888:
            return SDL_PIXELFORMAT_BGR888;
        case PIXFORMAT_BGRX8888:
            return SDL_PIXELFORMAT_BGRX8888;
        case PIXFORMAT_ARGB8888:
            return SDL_PIXELFORMAT_ARGB8888;
        case PIXFORMAT_RGBA8888:
            return SDL_PIXELFORMAT_RGBA8888;
        case PIXFORMAT_ABGR8888:
            return SDL_PIXELFORMAT_ABGR8888;
        case PIXFORMAT_BGRA8888:
            return SDL_PIXELFORMAT_BGRA8888;
        case PIXFORMAT_ARGB2101010:
            return SDL_PIXELFORMAT_ARGB2101010;
        case PIXFORMAT_RGBA32:
            return SDL_PIXELFORMAT_RGBA32;
        case PIXFORMAT_ARGB32:
            return SDL_PIXELFORMAT_ARGB32;
        case PIXFORMAT_BGRA32:
            return SDL_PIXELFORMAT_BGRA32;
        case PIXFORMAT_ABGR32:
            return SDL_PIXELFORMAT_ABGR32;
        case PIXFORMAT_YV12:
            return SDL_PIXELFORMAT_YV12;
        case PIXFORMAT_IYUV:
            return SDL_PIXELFORMAT_IYUV;
        case PIXFORMAT_YUY2:
            return SDL_PIXELFORMAT_UYVY;
        case PIXFORMAT_UYVY:
            return SDL_PIXELFORMAT_UYVY;
        case PIXFORMAT_YVYU:
            return SDL_PIXELFORMAT_YVYU;
        case PIXFORMAT_NV12:
            return SDL_PIXELFORMAT_NV12;
        case PIXFORMAT_NV21:
            return SDL_PIXELFORMAT_NV21; 
        default:
            return SDL_PIXELFORMAT_UNKNOWN;
    }
}

