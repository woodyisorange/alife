#include "game.h"

struct rgba32
{
    uint8 Red;
    uint8 Green;
    uint8 Blue;
    uint8 Alpha;
};

struct vector2f
{
    float32 X;
    float32 Y;

    float32& operator[](int32 Index)
    {
        //TODO: Bounds check
        return *(&X + Index);
    }
};

float32 Truncate(float32 A)
{
    return (float32)(int32)A;
}

vector2f Truncate(vector2f A)
{
    vector2f Result = {(float32)Truncate(A.X), (float32)Truncate(A.Y)};
    return Result;
}

float32 Dot(vector2f A, vector2f B)
{
    float32 Result = (A.X * B.X) + (A.Y * B.Y);
    return Result;
}

float32 LengthSquared(vector2f A)
{
    float32 Result = Dot(A, A);
    return Result;
}

float32 Length(vector2f A)
{
    float32 Result = __builtin_sqrtf(LengthSquared(A));
    return Result;
}

bool8 operator==(vector2f A, vector2f B)
{
    bool8 Result = A.X == B.X && A.Y == B.Y;
    return Result;
}

bool8 operator!=(vector2f A, vector2f B)
{
    bool8 Result = !(A == B);
    return Result;
}

vector2f operator*(vector2f A, float32 B)
{
    vector2f Result = { A.X * B, A.Y * B };
    return Result;
}

vector2f operator/(vector2f A, float32 B)
{
    float32 InverseB = 1.0f / B;
    vector2f Result = A * InverseB;
    return Result;
}

vector2f operator/=(vector2f& A, float32 B)
{
    A = A / B;
    return A;
}

vector2f operator-(vector2f A, vector2f B)
{
    vector2f Result = { A.X - B.X, A.Y - B.Y };
    return Result;
}

vector2f operator+(vector2f A, vector2f B)
{
    vector2f Result = { A.X + B.X, A.Y + B.Y };
    return Result;
}

vector2f operator+=(vector2f& A, vector2f B)
{
    A = A + B;
    return A;
}

vector2f Normalise(vector2f A)
{
    vector2f Result = A;
    float32 LengthA = Length(A);
    if (LengthA > 0)
    {
        Result /= LengthA;
    }
    return Result;
}

float32 Min(float32 A, float32 B)
{
    return (A < B) ? A : B;
}

float32 Max(float32 A, float32 B)
{
    return (A > B) ? A : B;
}

float32 Clamp(float32 Value, float32 MinValue, float32 MaxValue)
{
    float32 Result = Min(Max(Value, MinValue), MaxValue);
    return Result;
}

vector2f Clamp(vector2f Value, vector2f MinValue, vector2f MaxValue)
{
    vector2f Result;
    Result.X = Clamp(Value.X, MinValue.X, MaxValue.X);
    Result.Y = Clamp(Value.Y, MinValue.Y, MaxValue.Y);
    return Result;
}

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

void Game_DrawLine(bitmap* BackBuffer, rgba32 Colour, vector2f Start, vector2f End)
{
    vector2f ClampedStart = Clamp(Start, vector2f{ 0.0f, 0.0f }, vector2f{ (float32)BackBuffer->Width, (float32)BackBuffer->Height });
    vector2f ClampedEnd = Clamp(End, vector2f{ 0.0f, 0.0f }, vector2f{ (float32)BackBuffer->Width, (float32)BackBuffer->Height });
    vector2f Line = ClampedEnd - ClampedStart;
    float32 LineLength = Length(Line);
    vector2f Step = (LineLength > 0.0f) ? (Line / LineLength) : Line;

    vector2f Point = Start;
    for (int32 Steps = LineLength; Steps >= 0; --Steps)
    {
        Game_SetPixel(BackBuffer, Colour, Point.X, Point.Y);
        Point += Step;
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

    vector2f A = { BackBuffer->Width * 0.5f, BackBuffer->Height * 0.25f };
    vector2f B = { BackBuffer->Width * 0.75f, BackBuffer->Height * 0.75f };
    vector2f C = { BackBuffer->Width * 0.25f, BackBuffer->Height * 0.75f };

    rgba32 Colour = { 255, 0, 0, 255 };

    Game_DrawLine(BackBuffer, Colour, A, B);
    Game_DrawLine(BackBuffer, Colour, B, C);
    Game_DrawLine(BackBuffer, Colour, C, A);
}

