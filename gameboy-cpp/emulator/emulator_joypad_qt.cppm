export module emulator:joypad.qt;

import qt;
import std;
import joypad;
import utilities;

namespace emulator
{
    export class joypad_source
    {
    public:
        explicit joypad_source()
            : key_pressed_filter { qt::event_filter::create<qt::key_press_event>(
                qt::gui_application::instance(),
                [this] (const auto& event) { return on_key_pressed(event); }) }
            , key_released_filter { qt::event_filter::create<qt::key_release_event>(
                qt::gui_application::instance(),
                [this] (const auto& event) { return on_key_released(event); }) }
        {}

        [[nodiscard]] joypad::const_input_state_view_t read()
        {
            return state;
        }

    private:
        bool on_key_pressed(const qt::key_press_event& event)
        {
            update_key_state(event.key, true);
            return false;
        }

        bool on_key_released(const qt::key_release_event& event)
        {
            update_key_state(event.key, false);
            return false;
        }

        void update_key_state(const qt::key& key, const bool pressed)
        {
            using enum qt::key;

            switch (key)
            {
            case ret:
                state[0] = pressed;
                break;
            case backspace:
                state[1] = pressed;
                break;
            case up:
                state[2] = pressed;
                break;
            case down:
                state[3] = pressed;
                break;
            case left:
                state[4] = pressed;
                break;
            case right:
                state[5] = pressed;
                break;
            case x:
                state[6] = pressed;
                break;
            case z:
                state[7] = pressed;
                break;
            default: break;
            }
        }

        qt::event_filter key_pressed_filter;
        qt::event_filter key_released_filter;
        std::array<bool, joypad::num_joypad_inputs> state{};
    };
}
