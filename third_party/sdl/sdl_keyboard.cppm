module;
#include "SDL3/SDL_keyboard.h"

export module sdl:keyboard;
export import :scancode;
export import std;

import :internal;

namespace sdl
{
    export class keyboard_state
    {
    public:
        std::optional<bool> is_pressed(const scancode scancode) const
        {
            return is_pressed(std::to_underlying(scancode));
        }

        std::optional<bool> is_pressed(const scancode_t scancode) const
        {
            return scancode < state.size()
                ? std::make_optional(state[scancode])
                : std::nullopt;
        }

    private:
        explicit keyboard_state(const std::span<const bool> state)
            : state { state }
        {}

        friend internal::wrapper;
        std::span<const bool> state;
    };

    export [[nodiscard]] keyboard_state get_keyboard_state()
    {
        int sdl_num_available_keys {};
        const bool* sdl_state = SDL_GetKeyboardState(&sdl_num_available_keys);

        const std::span state { sdl_state, static_cast<std::size_t>(sdl_num_available_keys) };
        return internal::wrapper::create<keyboard_state>(state);
    }

}