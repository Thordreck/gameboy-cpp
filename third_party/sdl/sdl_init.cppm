
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
                return session(true);
            }

            return std::unexpected(SDL_GetError());
        }

        ~session()
        {
            if (valid) { SDL_Quit(); }
        }

        session(const session&) = delete;
        session& operator =(const session&) = delete;

        session(session&& other) noexcept
            : session { std::exchange(other.valid, false ) }
        {}

        session& operator=(session&& other) noexcept
        {
            valid = std::exchange(other.valid, false);
            return *this;
        }

    private:
        explicit session(const bool valid)
            : valid { valid }
        {}

        bool valid;
    };

}