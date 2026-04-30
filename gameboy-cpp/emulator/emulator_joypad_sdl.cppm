
export module emulator.joypad:sdl;

import sdl;
import std;
import joypad;
import emulator.core;

namespace emulator
{
    export class joypad_source
    {
    public:
        joypad_source()
            : source { sdl::get_keyboard_state() }
        {}

        [[nodiscard]] joypad::const_input_state_view_t read() const
        {
            using enum sdl::scancode;
            return std::array
            {
                source[enter],
                source[right_shift],
                source[up],
                source[down],
                source[left],
                source[right],
                source[x],
                source[z]
            };
        }

    private:
        sdl::keyboard_state source;
    };

}