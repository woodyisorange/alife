#include "platform.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const char* ProgramName = "tactile";

int32 StringCompare(const char* StringA, const char* StringB)
{
    if (StringA == NULL || StringB == NULL)
    {
        __builtin_debugtrap();
        __builtin_abort();
    }

    return __builtin_strcmp(StringA, StringB);
}

int32 main(int32 ArgumentCount, const char8* ArgumentValues[])
{
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
        goto sdl_fatal_error;
    }

    int32 WindowWidth = 800;
    int32 WindowHeight = 600;

    int32 WindowFlags = 0;
    WindowFlags |= WindowFullscreen ? SDL_WINDOW_FULLSCREEN : 0;

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

    bool8 IsRunning = true;
    while (IsRunning)
    {
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
    }

exit_program:
    SDL_Quit();
    return 0;

sdl_fatal_error:
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ProgramName, SDL_GetError(), NULL);
    goto exit_program;
}

