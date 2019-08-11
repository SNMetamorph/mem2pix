/*
    Renderer header
*/
#pragma once
#include <stdint.h>
#include <SDL.h>
#include "pixel_format.h"

class CRenderer
{
public:
    static CRenderer& Instance();
    void Init(
        uint32_t width, uint32_t height, 
        pixformat_t pixFormat, bool isBorderless
    );
    bool HandleEvents();
    void UpdateTexture();
    void DrawFrame();

    inline size_t GetFramebufferSize() { return m_iFramebufferSize; };
    inline uint8_t *GetFramebuffer()   { return m_pFramebuffer;     };

private:
    CRenderer();
    ~CRenderer();
    CRenderer(const CRenderer&) = delete;
    CRenderer& operator=(const CRenderer&) = delete;

    void Dispatch();
    bool AllocFramebuffer();
    void CalcFramebufferSize();
    SDL_PixelFormatEnum GetPixelFormat(
        pixformat_t pixFormat
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
};

// for suitable access to instance
CRenderer *renderer();