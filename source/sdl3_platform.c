#include "platform.h"

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const char* ProgramName = "tactile";

struct
{
    SDL_DateTime CurrentDateTime;
} PlatformGlobals;

#define INNER_AS_STRING(X) #X
#define AS_STRING(X) INNER_AS_STRING(X)

#define MAX(A, B) ((A) > (B) ? (A) : (B))

#define ENSURE(Expression) \
    { \
        if (!(Expression)) \
        { \
            __builtin_debugtrap(); \
            __builtin_abort(); \
        } \
    }

int64 FormatString(char8* Destination, int64 DestinationSize, char8* Format, ...)
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

#define LOG_MESSAGE(Format, ...) \
{ \
    SDL_Log( \
        "[%04d-%02d-%02d-%02d:%02d:%02d.%03d][" __FILE__ ":" AS_STRING(__LINE__) "] " Format, \
        PlatformGlobals.CurrentDateTime.year, \
        PlatformGlobals.CurrentDateTime.month, \
        PlatformGlobals.CurrentDateTime.day, \
        PlatformGlobals.CurrentDateTime.hour, \
        PlatformGlobals.CurrentDateTime.minute, \
        PlatformGlobals.CurrentDateTime.second, \
        (PlatformGlobals.CurrentDateTime.nanosecond / 1000000) \
        __VA_OPT__(,) __VA_ARGS__); \
}

int32 StringCompare(const char* StringA, const char* StringB)
{
    ENSURE(StringA != NULL && StringB != NULL);
    return __builtin_strcmp(StringA, StringB);
}

int32 main(int32 ArgumentCount, const char8* ArgumentValues[])
{
    SDL_Time CurrentTime;
    SDL_GetCurrentTime(&CurrentTime);
    SDL_TimeToDateTime(CurrentTime, &PlatformGlobals.CurrentDateTime, /*LocalTime*/true);

    LOG_MESSAGE("Starting %s", ProgramName);

    //
    // Parse Commandline
    //

    bool8 WindowFullscreen = true;

    for (int32 Index = 0; Index < ArgumentCount; ++Index)
    {
        if (StringCompare("-windowed", ArgumentValues[Index]) == 0)
        {
            WindowFullscreen = false;
        }
        else
        {
            //TODO: Handle unrecognised arguments
        }
    }

    //
    // Setup SDL
    //

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        goto sdl_fatal_error;
    }

    //
    // Setup window
    //

    int32 WindowWidth = 800;
    int32 WindowHeight = 600;

    int32 WindowFlags = 0;
    WindowFlags |= WindowFullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;

    SDL_Window* Window;
    SDL_Renderer* Renderer;

    if (!SDL_CreateWindowAndRenderer(ProgramName, WindowWidth, WindowHeight, WindowFlags, &Window, &Renderer))
    {
        goto sdl_fatal_error;
    }

    if (!SDL_GetWindowSize(Window, &WindowWidth, &WindowHeight))
    {
        goto sdl_fatal_error;
    }

    //
    // Main loop
    //

    int64 TargetFrameDurationNanoseconds = 16666666;

    bool8 IsRunning = true;
    int64 CurrentNanoseconds = SDL_GetTicksNS();
    while (IsRunning)
    {
        SDL_GetCurrentTime(&CurrentTime);
        SDL_TimeToDateTime(CurrentTime, &PlatformGlobals.CurrentDateTime, /*LocalTime*/true);

        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            switch (Event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    IsRunning = false;
                } break;

                case SDL_EVENT_KEY_UP:
                {
                    if (Event.key.key == SDLK_ESCAPE)
                    {
                        IsRunning = false;
                    }
                } break;
            }
        }

        SDL_SetRenderDrawColor(Renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(Renderer);
        SDL_RenderPresent(Renderer);

        int64 NewNanoseconds = SDL_GetTicksNS();
        int64 FrameDurationNanoseconds = NewNanoseconds - CurrentNanoseconds;
        CurrentNanoseconds = NewNanoseconds;

        int64 TotalFrameDurationNanoseconds = MAX(FrameDurationNanoseconds, TargetFrameDurationNanoseconds);

        float32 FrameDurationMilliseconds = (float32)FrameDurationNanoseconds / 1000000.0f;
        float32 TotalFrameDurationMilliseconds = (float32)TotalFrameDurationNanoseconds / 1000000.0f;
        float32 FrameRate = 1000.0f / TotalFrameDurationMilliseconds;

        char8 WindowTitle[256];
        FormatString(WindowTitle, sizeof(WindowTitle), "%s (Game: %04.1f ms | Frame: %04.1f ms | FPS: %04.1f)", ProgramName, FrameDurationMilliseconds, TotalFrameDurationMilliseconds, FrameRate);
        SDL_SetWindowTitle(Window, WindowTitle);

        int64 NanosecondsToEndOfFrame = TargetFrameDurationNanoseconds - FrameDurationNanoseconds;
        if (NanosecondsToEndOfFrame > 0)
        {
            SDL_DelayPrecise(NanosecondsToEndOfFrame);
        }
    }

exit_program:
    SDL_Quit();
    return 0;

sdl_fatal_error:
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ProgramName, SDL_GetError(), NULL);
    goto exit_program;
}

