#include <platform/types.h>
#include <utility/linear_algebra.h>
#include <utility/math.h>
#include <utility/string.h>

#define INNER_AS_STRING(X) #X
#define AS_STRING(X) INNER_AS_STRING(X)

#define ENSURE(Expression) \
    { \
        if (!(Expression)) \
        { \
            __builtin_debugtrap(); \
            __builtin_abort(); \
        } \
    }

#ifdef BUILD_DEBUG
#define ASSERT(Expression) \
    { \
        if (!(Expression)) \
        { \
            __builtin_debugtrap(); \
            __builtin_abort(); \
        } \
    }
#else
#define ASSERT(Expression)
#endif

