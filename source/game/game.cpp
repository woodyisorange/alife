#include <game/game.h>
#include <utility/linear_algebra.h>

struct rgba32
{
    uint8 Red;
    uint8 Green;
    uint8 Blue;
    uint8 Alpha;
};

void Game_SetPixel(bitmap* BackBuffer, rgba32 Colour, int32 X, int32 Y)
{
    //TODO asserts/ensures
    if (BackBuffer->PixelFormat != PIXEL_FORMAT_ARGB8888)
    {
        __builtin_abort();
    }

    if (X >= 0 && X < BackBuffer->Width && Y >= 0 && Y < BackBuffer->Height)
    {
        uint8* BackBufferRow = (uint8*)BackBuffer->Pixels + (BackBuffer->Pitch * Y);
        rgba32* Pixel = (rgba32*)(BackBufferRow + (4 * X));
        *Pixel = Colour;
    }
}

void Game_DrawLine(bitmap* BackBuffer, rgba32 Colour, vector2 Start, vector2 End)
{
    vector2 Line = End - Start;
    float32 LineLength = Length(Line);
    vector2 Step = (LineLength > 0.0f) ? (Line / LineLength) : Line;

    vector2 Point = Start;
    for (int32 Steps = LineLength; Steps >= 0; --Steps)
    {
        Game_SetPixel(BackBuffer, Colour, Point.X, Point.Y);
        Point += Step;
    }
}

void Game_DrawTriangle(bitmap* BackBuffer, rgba32 Colour, vector2 A, vector2 B, vector2 C)
{
    Game_DrawLine(BackBuffer, Colour, A, B);
    Game_DrawLine(BackBuffer, Colour, B, C);
    Game_DrawLine(BackBuffer, Colour, C, A);
}

void Game_FillTriangle(bitmap* BackBuffer, rgba32 Colour, vector2 A, vector2 B, vector2 C)
{
    vector3 AToB = Vector3(B - A, 0.0f);
    vector3 AToC = Vector3(C - A, 0.0f);
    vector3 BToC = Vector3(C - B, 0.0f);

    int32 MinX = (int32)Min(A.X, B.X, C.X);
    MinX = Clamp(MinX, 0, BackBuffer->Width);

    int32 MaxX = (int32)Max(A.X, B.X, C.X) + 1;
    MaxX = Clamp(MaxX, 0, BackBuffer->Width);

    int32 MinY = (int32)Min(A.Y, B.Y, C.Y);
    MinY = Clamp(MinY, 0, BackBuffer->Height);

    int32 MaxY = (int32)Max(A.Y, B.Y, C.Y) + 1;
    MaxY = Clamp(MaxY, 0, BackBuffer->Height);

    for (int32 Y = MinY; Y < MaxY; ++Y)
    {
        for (int32 X = MinX; X < MaxX; ++X)
        {
            vector2 Point = { (float32)X, (float32)Y };
            vector3 AToPoint = Vector3(Point - A, 0.0f);
            vector3 BToPoint = Vector3(Point - A, 0.0f);

            bool8 InsideAToB = Cross(AToB, AToPoint).Z >= 0.0f;
            bool8 InsideAToC = Cross(AToC, AToPoint).Z <= 0.0f;
            bool8 InsideBToC = Cross(BToC, BToPoint).Z <= 0.0f;

            bool8 InsideTriangle = InsideAToB && InsideAToC && InsideBToC;
            if (InsideTriangle)
            {
                //TODO: Track pixel address locally
                Game_SetPixel(BackBuffer, Colour, X, Y);
            }
        }
    }
}

void Game_UpdateAndRender(bitmap* BackBuffer)
{
    //TODO asserts/ensures
    if (BackBuffer->PixelFormat != PIXEL_FORMAT_ARGB8888)
    {
        __builtin_abort();
    }

    uint8* BackBufferRow = (uint8*)BackBuffer->Pixels;
    rgba32 ClearColour = { 0, 0, 0, 255 };
    for (int32 Y = 0; Y < BackBuffer->Height; ++Y)
    {
        for (int32 X = 0; X < BackBuffer->Width; ++X)
        {
            rgba32* Pixel = (rgba32*)(BackBufferRow + (4 * X));
            *Pixel = ClearColour;
        }
        BackBufferRow += BackBuffer->Pitch;
    }

    vector2 A = { BackBuffer->Width * 0.5f, BackBuffer->Height * 0.25f };
    vector2 B = { BackBuffer->Width * 0.75f, BackBuffer->Height * 0.75f };
    vector2 C = { BackBuffer->Width * 0.25f, BackBuffer->Height * 0.75f };

    rgba32 Red  = { 255, 0, 0, 255 };
    rgba32 Blue  = { 0, 0, 255, 255 };

    Game_FillTriangle(BackBuffer, Blue, A, B, C);
    Game_DrawTriangle(BackBuffer, Red, A, B, C);
}

