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

enum class PixelFormat
{
    Invalid,
	Grayscale8,
    RGB332,
    RGB444,
    RGB555,
    BGR555,
    ARGB4444,
    RGBA4444,
    ABGR4444,
    BGRA4444,
    ARGB1555,
    RGBA5551,
    ABGR1555,
    BGRA5551,
    RGB565,
    BGR565,
    RGB24,
    BGR24,
    RGB888,
    RGBX8888,
    BGR888,
    BGRX8888,
    ARGB8888,
    RGBA8888,
    ABGR8888,
    BGRA8888,
    ARGB2101010,
    RGBA32,
    ARGB32,
    BGRA32,
    ABGR32,
    YV12,
    IYUV,
    YUY2,
    UYVY,
    YVYU,
    NV12,
    NV21,
};
