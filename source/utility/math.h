#pragma once
#include <platform/types.h>

template <typename generic>
inline generic Min(generic A, generic B)
{
    return A < B ? A : B;
}

template <typename generic>
inline generic Max(generic A, generic B)
{
    return A > B ? A : B;
}

template <typename generic>
inline generic Min(generic A, generic B, generic C)
{
    return Min(Min(A, B), C);
}

template <typename generic>
inline generic Max(generic A, generic B, generic C)
{
    return Max(Max(A, B), C);
}

template <typename generic>
inline generic Clamp(generic Value, generic MinValue, generic MaxValue)
{
    generic Result = Min(Max(Value, MinValue), MaxValue);
    return Result;
}

inline float32 Sqrt(float32 Value)
{
    return __builtin_sqrtf(Value);
}

