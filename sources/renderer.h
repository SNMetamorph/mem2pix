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
