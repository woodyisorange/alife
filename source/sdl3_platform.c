#include "platform.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const char* program_name = "tactile";

int32 string_compare(const char* string_a, const char* string_b)
{
    if (string_a == NULL || string_b == NULL)
    {
        __builtin_debugtrap();
        __builtin_abort();
    }

    return __builtin_strcmp(string_a, string_b);
}

int32 main(int32 argc, const char8* argv[])
{
    //
    // Parse Commandline
    //

    bool8 window_fullscreen = false;

    for (int32 arg_index = 0; arg_index < argc; ++arg_index)
    {
        if (string_compare("-fullscreen", argv[arg_index]) == 0)
        {
            window_fullscreen = true;
        }
    }

    //
    // Setup SDL
    //

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        goto sdl_fatal_error;
    }

    int32 window_width = 800;
    int32 window_height = 600;

    int32 window_flags = 0;
    window_flags |= window_fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (!SDL_CreateWindowAndRenderer(program_name, window_width, window_height, window_flags, &window, &renderer))
    {
        goto sdl_fatal_error;
    }

    if (!SDL_GetWindowSize(window, &window_width, &window_height))
    {
        goto sdl_fatal_error;
    }

    //
    // TODO: Main loop
    //

    SDL_Delay(5000);

exit_program:
    SDL_Quit();
    return 0;

sdl_fatal_error:
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, program_name, SDL_GetError(), NULL);
    goto exit_program;
}

