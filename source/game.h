#pragma once
#include "common.h"

enum pixel_format
{
    PIXEL_FORMAT_ARGB8888
};

struct bitmap
{
    pixel_format PixelFormat;
    int32 Width;
    int32 Height;
    int32 Pitch;
    void* Pixels;
};

void Game_UpdateAndRender(bitmap* BackBuffer);

