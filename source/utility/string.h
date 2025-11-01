#pragma once
#include <utility/common.h>

inline void* MemCopy(void* Destination, void* Source, int64 Count)
{
    return __builtin_memcpy(Destination, Source, Count);
}

inline int32 MemCompare(void* Destination, void* Source, int64 Count)
{
    return __builtin_memcmp(Destination, Source, Count);
}

inline int32 StringCompare(const char* StringA, const char* StringB)
{
    return __builtin_strcmp(StringA, StringB);
}

int64 FormatString(char8* Destination, int64 DestinationSize, const char8* Format, ...);

