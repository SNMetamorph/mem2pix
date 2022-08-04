/*
    Renderer header
*/
#pragma once
#include "pixel_format.h"
#include <stdint.h>
#include <SDL.h>
#include <string>

class CRenderer
{
public:
    CRenderer();
    ~CRenderer();

    void Init(
        uint32_t width, uint32_t height, 
        PixelFormat pixFormat, bool isBorderless
    );
    void UpdateWindowTitle(const std::string &title);
    bool HandleEvents();
    void UpdateTexture();
    void DrawFrame();

    inline size_t GetFramebufferSize() { return m_iFramebufferSize; };
    inline uint8_t *GetFramebuffer()   { return m_pFramebuffer;     };

private:
    void Dispatch();
    bool AllocFramebuffer();
    void CalcFramebufferSize();
    SDL_PixelFormatEnum GetPixelFormat(
        PixelFormat pixFormat
    );

    SDL_Window      *m_pWindow;
    SDL_Renderer    *m_pRenderer;
    SDL_Texture     *m_pTexture;
    uint8_t         *m_pFramebuffer;
    size_t          m_iFramebufferSize;
    uint32_t        m_iWindowWidth;
    uint32_t        m_iWindowHeight;
    uint32_t        m_iRefreshRate;
    bool            m_isFollowCursor;
    bool            m_isLegacyDragMethod;
    std::string     m_szWindowTitle;
};
