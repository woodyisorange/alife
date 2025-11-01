#include <utility/string.h>

#include <utility/common.h>

#include <stdarg.h>

//TODO Remove stdlib dependencies
#include <stdio.h>

int64 FormatString(char8* Destination, int64 DestinationSize, const char8* Format, ...)
{
    if (Destination == NULL || DestinationSize <= 0)
    {
        return 0;
    }

    va_list Arguments;
    va_start(Arguments, Format);

    int32 Length = vsprintf(Destination, Format, Arguments);

    ENSURE(Length >= 0 && Length <= DestinationSize);
    return Length;
}

