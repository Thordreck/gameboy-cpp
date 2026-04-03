
module;
#include <SDL3/SDL_init.h>

export module sdl:init;
export import :common;
export import std;

namespace sdl
{
    export enum class init_flags : std::uint32_t
    {
        audio = SDL_INIT_AUDIO,
        video = SDL_INIT_VIDEO,
        joystick = SDL_INIT_JOYSTICK,
        haptic = SDL_INIT_HAPTIC,
        gamepad = SDL_INIT_GAMEPAD,
        events = SDL_INIT_EVENTS,
        sensor = SDL_INIT_SENSOR,
        camera = SDL_INIT_CAMERA,
    };

    export class session
    {
    public:
        static result<session> create(const init_flags flags)
        {
            if (SDL_Init(std::to_underlying(flags)))
            {
                return session();
            }

            return std::unexpected(SDL_GetError());
        }

        ~session()
        {
            SDL_Quit();
        }

        // TODO: delete copy constructors. Implement proper move semantics

    };

}