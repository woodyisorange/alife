#include <game/game.h>
#include <utility/common.h>

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const char* ProgramName = "alife";

struct
{
    SDL_DateTime CurrentDateTime;
} PlatformGlobals;

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

int32 main(int32 ArgumentCount, const char8* ArgumentValues[])
{
    SDL_Time CurrentTime;
    SDL_GetCurrentTime(&CurrentTime);
    SDL_TimeToDateTime(CurrentTime, &PlatformGlobals.CurrentDateTime, /*LocalTime*/true);

    LOG_MESSAGE("Starting %s", ProgramName);

    //
    // Parse Commandline
    //

    bool8 WindowFullscreen = false;

    for (int32 Index = 0; Index < ArgumentCount; ++Index)
    {
        if (StringCompare("-fullscreen", ArgumentValues[Index]) == 0)
        {
            WindowFullscreen = true;
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
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ProgramName, SDL_GetError(), NULL);
        SDL_Quit();
        __builtin_abort();
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
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ProgramName, SDL_GetError(), NULL);
        SDL_Quit();
        __builtin_abort();
    }

    if (!SDL_GetRenderOutputSize(Renderer, &WindowWidth, &WindowHeight))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ProgramName, SDL_GetError(), NULL);
        SDL_Quit();
        __builtin_abort();
    }

    int32 BackBufferWidth = WindowWidth;
    int32 BackBufferHeight = WindowHeight;
    SDL_Texture* BackBuffer = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, BackBufferWidth, BackBufferHeight);
    if (!BackBuffer)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ProgramName, SDL_GetError(), NULL);
        SDL_Quit();
        __builtin_abort();
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

        //
        // If the window has resized, we'll need to resize our backbuffer
        //

        if (!SDL_GetRenderOutputSize(Renderer, &WindowWidth, &WindowHeight))
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ProgramName, SDL_GetError(), NULL);
            SDL_Quit();
            __builtin_abort();
        }

        if (WindowWidth != BackBufferWidth || WindowHeight != BackBufferHeight)
        {
            BackBufferWidth = WindowWidth;
            BackBufferHeight = WindowHeight;

            SDL_DestroyTexture(BackBuffer);
            BackBuffer = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, BackBufferWidth, BackBufferHeight);

            if (!BackBuffer)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ProgramName, SDL_GetError(), NULL);
                SDL_Quit();
                __builtin_abort();
            }
        }

        //
        // Draw Frame
        //

        bitmap GameBackBuffer;
        GameBackBuffer.PixelFormat = PIXEL_FORMAT_ARGB8888;
        GameBackBuffer.Width = BackBufferWidth;
        GameBackBuffer.Height = BackBufferHeight;
        SDL_LockTexture(BackBuffer, NULL, &GameBackBuffer.Pixels, &GameBackBuffer.Pitch);

        Game_UpdateAndRender(&GameBackBuffer);

        //
        // Finalise Frame
        //

        SDL_UnlockTexture(BackBuffer);
        SDL_RenderTexture(Renderer, BackBuffer, NULL, NULL);
        SDL_RenderPresent(Renderer);

        //
        // Update timing information
        //

        int64 NewNanoseconds = SDL_GetTicksNS();
        int64 FrameDurationNanoseconds = NewNanoseconds - CurrentNanoseconds;
        CurrentNanoseconds = NewNanoseconds;

        int64 TotalFrameDurationNanoseconds = Max(FrameDurationNanoseconds, TargetFrameDurationNanoseconds);

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

    SDL_Quit();
    return 0;
}

