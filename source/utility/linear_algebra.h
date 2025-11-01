#pragma once
#include <platform/types.h>
#include <utility/string.h>
#include <utility/math.h>

struct vector2
{
    float32 X;
    float32 Y;

    inline float32& operator[](int32 Index)
    {
        //TODO: Bounds check
        return (&X)[Index];
    }
};

inline vector2 operator-(vector2 RHS)
{
    vector2 Result;
    for (int32 Index = 0; Index < 2; ++Index)
    {
        Result[Index] = -RHS[Index];
    }
    return Result;
}

inline vector2 operator-(vector2 LHS, vector2 RHS)
{
    vector2 Result;
    for (int32 Index = 0; Index < 2; ++Index)
    {
        Result[Index] = LHS[Index] - RHS[Index];
    }
    return Result;
}

inline vector2 operator-=(vector2& LHS, vector2 RHS)
{
    for (int32 Index = 0; Index < 2; ++Index)
    {
        LHS[Index] -= RHS[Index];
    }
    return LHS;
}

inline vector2 operator+(vector2 LHS, vector2 RHS)
{
    vector2 Result;
    for (int32 Index = 0; Index < 2; ++Index)
    {
        Result[Index] = LHS[Index] + RHS[Index];
    }
    return Result;
}

inline vector2 operator+=(vector2& LHS, vector2 RHS)
{
    for (int32 Index = 0; Index < 2; ++Index)
    {
        LHS[Index] += RHS[Index];
    }
    return LHS;
}

inline vector2 operator*(vector2 LHS, float32 RHS)
{
    vector2 Result;
    for (int32 Index = 0; Index < 2; ++Index)
    {
        Result[Index] = LHS[Index] * RHS;
    }
    return Result;
}

inline vector2 operator*(float32 LHS, vector2 RHS)
{
    vector2 Result;
    for (int32 Index = 0; Index < 2; ++Index)
    {
        Result[Index] = LHS * RHS[Index];
    }
    return Result;
}

inline vector2 operator*=(vector2& LHS, float32 RHS)
{
    for (int32 Index = 0; Index < 2; ++Index)
    {
        LHS[Index] *= RHS;
    }
    return LHS;
}

inline vector2 operator/(vector2 LHS, float32 RHS)
{
    vector2 Result;
    for (int32 Index = 0; Index < 2; ++Index)
    {
        Result[Index] = LHS[Index] / RHS;
    }
    return Result;
}

inline vector2 operator/=(vector2& LHS, float32 RHS)
{
    for (int32 Index = 0; Index < 2; ++Index)
    {
        LHS[Index] /= RHS;
    }
    return LHS;
}

inline float32 Dot(vector2 LHS, vector2 RHS)
{
    float32 Result = 0;
    for (int32 Index = 0; Index < 2; ++Index)
    {
        Result += LHS[Index] * RHS[Index];
    }
    return Result;
}

inline float32 LengthSquared(vector2 A)
{
    float32 Result = Dot(A, A);
    return Result;
}

inline float32 Length(vector2 A)
{
    float32 Result = Sqrt(LengthSquared(A));
    return Result;
}

inline bool8 IsNearlyZero(vector2 A)
{
    constexpr float32 Epsilon = 0.000001;
    for (int32 Index = 0; Index < 2; ++Index)
    {
        if (A[Index] > Epsilon || A[Index] < -Epsilon)
        {
            return false;
        }
    }
    return true;
}

struct vector3
{
    float32 X;
    float32 Y;
    float32 Z;

    inline float32& operator[](int32 Index)
    {
        //TODO: Bounds check
        return (&X)[Index];
    }
};

inline vector3 operator-(vector3 RHS)
{
    vector3 Result;
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Result[Index] = -RHS[Index];
    }
    return Result;
}

inline vector3 operator-(vector3 LHS, vector3 RHS)
{
    vector3 Result;
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Result[Index] = LHS[Index] - RHS[Index];
    }
    return Result;
}

inline vector3 operator-=(vector3& LHS, vector3 RHS)
{
    for (int32 Index = 0; Index < 3; ++Index)
    {
        LHS[Index] -= RHS[Index];
    }
    return LHS;
}

inline vector3 operator+(vector3 LHS, vector3 RHS)
{
    vector3 Result;
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Result[Index] = LHS[Index] + RHS[Index];
    }
    return Result;
}

inline vector3 operator+=(vector3& LHS, vector3 RHS)
{
    for (int32 Index = 0; Index < 3; ++Index)
    {
        LHS[Index] += RHS[Index];
    }
    return LHS;
}

inline vector3 operator*(vector3 LHS, float32 RHS)
{
    vector3 Result;
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Result[Index] = LHS[Index] * RHS;
    }
    return Result;
}

inline vector3 operator*(float32 LHS, vector3 RHS)
{
    vector3 Result;
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Result[Index] = LHS * RHS[Index];
    }
    return Result;
}

inline vector3 operator*=(vector3& LHS, float32 RHS)
{
    for (int32 Index = 0; Index < 3; ++Index)
    {
        LHS[Index] *= RHS;
    }
    return LHS;
}

inline vector3 operator/(vector3 LHS, float32 RHS)
{
    vector3 Result;
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Result[Index] = LHS[Index] / RHS;
    }
    return Result;
}

inline vector3 operator/=(vector3& LHS, float32 RHS)
{
    for (int32 Index = 0; Index < 3; ++Index)
    {
        LHS[Index] /= RHS;
    }
    return LHS;
}

inline float32 Dot(vector3 LHS, vector3 RHS)
{
    float32 Result = 0;
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Result += LHS[Index] * RHS[Index];
    }
    return Result;
}

inline vector3 Cross(vector3 LHS, vector3 RHS)
{
    vector3 Result;
    Result.X = (LHS.Y * RHS.Z) - (LHS.Z * RHS.Y);
    Result.Y = (LHS.Z * RHS.X) - (LHS.X * RHS.Z);
    Result.Z = (LHS.X * RHS.Y) - (LHS.Y * RHS.X);
    return Result;
}

inline float32 LengthSquared(vector3 A)
{
    float32 Result = Dot(A, A);
    return Result;
}

inline float32 Length(vector3 A)
{
    float32 Result = Sqrt(LengthSquared(A));
    return Result;
}

inline bool8 IsNearlyZero(vector3 A)
{
    constexpr float32 Epsilon = 0.000001;
    for (int32 Index = 0; Index < 3; ++Index)
    {
        if (A[Index] > Epsilon || A[Index] < -Epsilon)
        {
            return false;
        }
    }
    return true;
}

struct vector4
{
    float32 X;
    float32 Y;
    float32 Z;
    float32 W;

    inline float32& operator[](int32 Index)
    {
        //TODO: Bounds check
        return (&X)[Index];
    }
};

inline vector4 operator-(vector4 RHS)
{
    vector4 Result;
    for (int32 Index = 0; Index < 4; ++Index)
    {
        Result[Index] = -RHS[Index];
    }
    return Result;
}

inline vector4 operator-(vector4 LHS, vector4 RHS)
{
    vector4 Result;
    for (int32 Index = 0; Index < 4; ++Index)
    {
        Result[Index] = LHS[Index] - RHS[Index];
    }
    return Result;
}

inline vector4 operator-=(vector4& LHS, vector4 RHS)
{
    for (int32 Index = 0; Index < 4; ++Index)
    {
        LHS[Index] -= RHS[Index];
    }
    return LHS;
}

inline vector4 operator+(vector4 LHS, vector4 RHS)
{
    vector4 Result;
    for (int32 Index = 0; Index < 4; ++Index)
    {
        Result[Index] = LHS[Index] + RHS[Index];
    }
    return Result;
}

inline vector4 operator+=(vector4& LHS, vector4 RHS)
{
    for (int32 Index = 0; Index < 4; ++Index)
    {
        LHS[Index] += RHS[Index];
    }
    return LHS;
}

inline vector4 operator*(vector4 LHS, float32 RHS)
{
    vector4 Result;
    for (int32 Index = 0; Index < 4; ++Index)
    {
        Result[Index] = LHS[Index] * RHS;
    }
    return Result;
}

inline vector4 operator*(float32 LHS, vector4 RHS)
{
    vector4 Result;
    for (int32 Index = 0; Index < 4; ++Index)
    {
        Result[Index] = LHS * RHS[Index];
    }
    return Result;
}

inline vector4 operator*=(vector4& LHS, float32 RHS)
{
    for (int32 Index = 0; Index < 4; ++Index)
    {
        LHS[Index] *= RHS;
    }
    return LHS;
}

inline vector4 operator/(vector4 LHS, float32 RHS)
{
    vector4 Result;
    for (int32 Index = 0; Index < 4; ++Index)
    {
        Result[Index] = LHS[Index] / RHS;
    }
    return Result;
}

inline vector4 operator/=(vector4& LHS, float32 RHS)
{
    for (int32 Index = 0; Index < 4; ++Index)
    {
        LHS[Index] /= RHS;
    }
    return LHS;
}

inline float32 Dot(vector4 LHS, vector4 RHS)
{
    float32 Result = 0;
    for (int32 Index = 0; Index < 4; ++Index)
    {
        Result += LHS[Index] * RHS[Index];
    }
    return Result;
}

inline float32 LengthSquared(vector4 A)
{
    float32 Result = Dot(A, A);
    return Result;
}

inline float32 Length(vector4 A)
{
    float32 Result = Sqrt(LengthSquared(A));
    return Result;
}

inline bool8 IsNearlyZero(vector4 A)
{
    constexpr float32 Epsilon = 0.000001;
    for (int32 Index = 0; Index < 4; ++Index)
    {
        if (A[Index] > Epsilon || A[Index] < -Epsilon)
        {
            return false;
        }
    }
    return true;
}

inline vector2 Vector2(vector3 Other)
{
    vector2 Result;
    Result.X = Other.X;
    Result.Y = Other.Y;
    return Result;
}

inline vector2 Vector2(vector4 Other)
{
    vector2 Result;
    Result.X = Other.X;
    Result.Y = Other.Y;
    return Result;
}

inline vector3 Vector3(vector2 Other, float32 Z)
{
    vector3 Result;
    Result.X = Other.X;
    Result.Y = Other.Y;
    Result.Z = Z;
    return Result;
}

inline vector3 Vector3(vector4 Other)
{
    vector3 Result;
    Result.X = Other.X;
    Result.Y = Other.Y;
    Result.Z = Other.Z;
    return Result;
}

inline vector4 Vector4(vector2 Other, float32 Z, float32 W)
{
    vector4 Result;
    Result.X = Other.X;
    Result.Y = Other.Y;
    Result.Z = Z;
    Result.W = W;
    return Result;
}

inline vector4 Vector4(vector3 Other, float32 W)
{
    vector4 Result;
    Result.X = Other.X;
    Result.Y = Other.Y;
    Result.Z = Other.Z;
    Result.W = W;
    return Result;
}

