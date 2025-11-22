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

void Game_DrawDeviceSpaceLine(bitmap* BackBuffer, rgba32 Colour, vector3 Start, vector3 End)
{
    vector2 Line = MakeVector2(End - Start);
    float32 LineLength = Length(Line);
    vector2 Step = (LineLength > 0.0f) ? (Line / LineLength) : Line;

    vector2 Point = MakeVector2(Start);
    for (int32 Steps = LineLength; Steps >= 0; --Steps)
    {
        Game_SetPixel(BackBuffer, Colour, Point.X, Point.Y);
        Point += Step;
    }
}

void Game_DrawDeviceSpaceTriangle(bitmap* BackBuffer, rgba32 Colour, vector3 A, vector3 B, vector3 C)
{
    Game_DrawDeviceSpaceLine(BackBuffer, Colour, A, B);
    Game_DrawDeviceSpaceLine(BackBuffer, Colour, B, C);
    Game_DrawDeviceSpaceLine(BackBuffer, Colour, C, A);
}

void Game_FillDeviceSpaceTriangle(bitmap* BackBuffer, rgba32 Colour, vector3 A, vector3 B, vector3 C)
{
    vector3 AToB = B - A;
    vector3 AToC = C - A;
    vector3 BToC = C - B;

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
            // Test center of pixel
            vector3 Point = { (float32)X + 0.5f, (float32)Y + 0.5f, 0.0f };

            vector3 AToPoint = Point - A;
            vector3 BToPoint = Point - B;

            bool8 InsideAToB = Cross(AToB, AToPoint).Z >= 0.0f;
            bool8 InsideAToC = Cross(AToC, AToPoint).Z <= 0.0f;
            bool8 InsideBToC = Cross(BToC, BToPoint).Z >= 0.0f;

            bool8 InsideTriangle = InsideAToB && InsideAToC && InsideBToC;
            if (InsideTriangle)
            {
                //TODO: Track pixel address locally
                Game_SetPixel(BackBuffer, Colour, X, Y);
            }
        }
    }
}

vector3 Game_ClipSpaceToDeviceSpace(bitmap* BackBuffer, vector3 Point)
{
    vector3 DeviceSpacePoint;
    DeviceSpacePoint.X = BackBuffer->Width * (Point.X + 0.5f);
    DeviceSpacePoint.Y = BackBuffer->Height * (1.0f - (Point.Z + 0.5f));
    DeviceSpacePoint.Z = 0.0f;
    return DeviceSpacePoint;
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

    // Left-handed, +X=Right, +Y=Forward, +Z=Up
    vector3 A = {  0.00f,  0.00f,  0.25f };
    vector3 B = {  0.25f,  0.00f, -0.25f };
    vector3 C = { -0.25f,  0.00f,  0.00f };

    vector3 DeviceSpaceA = Game_ClipSpaceToDeviceSpace(BackBuffer, A);
    vector3 DeviceSpaceB = Game_ClipSpaceToDeviceSpace(BackBuffer, B);
    vector3 DeviceSpaceC = Game_ClipSpaceToDeviceSpace(BackBuffer, C);

    rgba32 Red  = { 255, 0, 0, 255 };
    rgba32 Blue  = { 0, 0, 255, 255 };

    Game_FillDeviceSpaceTriangle(BackBuffer, Blue, DeviceSpaceA, DeviceSpaceB, DeviceSpaceC);
    Game_DrawDeviceSpaceTriangle(BackBuffer, Red, DeviceSpaceA, DeviceSpaceB, DeviceSpaceC);
}

