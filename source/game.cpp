#include "game.h"

void Game_UpdateAndRender(void* BackBufferPixels, int32 BackBufferWidth, int32 BackBufferHeight, int32 BackBufferPitch)
{
    uint8* BackBufferRow = (uint8*)BackBufferPixels;
    for (int32 Y = 0; Y < BackBufferHeight; ++Y)
    {
        for (int32 X = 0; X < BackBufferWidth; ++X)
        {
            uint8* Red = BackBufferRow + (4 * X);
            uint8* Green = Red + 1;
            uint8* Blue = Red + 2;
            uint8* Alpha = Red + 3;

            *Alpha = 255;
            *Red = 0;
            *Green = X % 256;
            *Blue = Y % 256;
        }
        BackBufferRow += BackBufferPitch;
    }
}

