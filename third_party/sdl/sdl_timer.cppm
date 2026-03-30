module;
#include <SDL3/SDL_timer.h>

export module sdl:timer;
export import std;

namespace sdl
{
    export void delay(const std::chrono::milliseconds& ms)
    {
        SDL_Delay(ms.count());
    }

}
