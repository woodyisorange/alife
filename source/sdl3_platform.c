#include "platform.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int32 main(int32 argc, const char8* argv[])
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "tactile", "hello, world!", NULL);
    if (argc > 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "tactile", argv[0], NULL);
    }
}

