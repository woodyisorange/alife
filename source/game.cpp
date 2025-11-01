#include "game.h"

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

void Game_SetPixel(void* BackBufferPixels, int32 BackBufferWidth, int32 BackBufferHeight, int32 BackBufferPitch, int32 X, int32 Y, uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
{
    if (X >= 0 && X < BackBufferWidth && Y >= 0 && Y < BackBufferHeight)
    {
        uint8* BackBufferRow = (uint8*)BackBufferPixels + (BackBufferPitch * Y);
        uint32* Pixel = (uint32*)(BackBufferRow + (4 * X));
        *Pixel = (Alpha << 24) | (Blue << 16) | (Green << 8) | (Red << 0);
    }
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

void Game_DrawLine(vector2f Start, vector2f End, void* BackBufferPixels, int32 BackBufferWidth, int32 BackBufferHeight, int32 BackBufferPitch, uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
{
    vector2f ClampedStart = Clamp(Start, vector2f{ 0.0f, 0.0f }, vector2f{ (float32)BackBufferWidth, (float32)BackBufferHeight });
    vector2f ClampedEnd = Clamp(Start, vector2f{ 0.0f, 0.0f }, vector2f{ (float32)BackBufferWidth, (float32)BackBufferHeight });
    vector2f Line = ClampedEnd - ClampedStart;
    float32 LineLength = Length(Line);
    vector2f Step = (LineLength > 0.0f) ? (Line / LineLength) : Line;

    vector2f Point = Start;
    for (int32 Steps = LineLength; Steps >= 0; --Steps)
    {
        Game_SetPixel(BackBufferPixels, BackBufferWidth, BackBufferHeight, BackBufferPitch, Point.X, Point.Y, Red, Green, Blue, Alpha);
        Point += Step;
    }
}

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
            *Green = 0;
            *Blue = 0;
        }
        BackBufferRow += BackBufferPitch;
    }

    vector2f A = { BackBufferWidth * 0.5f, BackBufferHeight * 0.25f };
    vector2f B = { BackBufferWidth * 0.75f, BackBufferHeight * 0.75f };
    vector2f C = { BackBufferWidth * 0.25f, BackBufferHeight * 0.75f };
    Game_DrawLine(A, B, BackBufferPixels, BackBufferWidth, BackBufferHeight, BackBufferPitch, 255, 0, 0, 255);
    Game_DrawLine(B, C, BackBufferPixels, BackBufferWidth, BackBufferHeight, BackBufferPitch, 255, 0, 0, 255);
    Game_DrawLine(C, A, BackBufferPixels, BackBufferWidth, BackBufferHeight, BackBufferPitch, 255, 0, 0, 255);
}

