export module emulator.joypad:qt;

import std;
import joypad;
import emulator.core;

namespace emulator
{
    export class joypad_source
    {
    public:
        [[nodiscard]] joypad::const_input_state_view_t read() const
        {
            // TODO: implement
            return std::array<bool, joypad::num_joypad_inputs> {};
        }
    };

}
