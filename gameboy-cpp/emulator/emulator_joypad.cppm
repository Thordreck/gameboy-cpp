export module emulator.core:joypad;

import std;
import joypad;

namespace emulator
{
    export struct joypad_input_state
    {
        bool start{};
        bool select{};
        bool up{};
        bool down{};
        bool left{};
        bool right{};
        bool b{};
        bool a{};
    };

    export class joypad_input_state_provider
    {
    public:
        explicit joypad_input_state_provider(joypad_input_state& input_state)
            : state{input_state}
        {
        }

        [[nodiscard]] bool is_input_pressed(const joypad::joypad_input input) const
        {
            using enum joypad::joypad_input;

            switch (input)
            {
            case start: return state.start;
            case select: return state.select;
            case up: return state.up;
            case down: return state.down;
            case left: return state.left;
            case right: return state.right;
            case b: return state.b;
            case a: return state.a;
            default: std::unreachable();
            }
        }

    private:
        joypad_input_state& state;
    };

}