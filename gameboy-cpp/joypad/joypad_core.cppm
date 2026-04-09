
export module joypad:core;

import :common;
import :memory;

import std;
import interrupts;

namespace joypad
{
    static bool detect_input_bits_falling_edge(const memory::memory_data_t previous, const memory::memory_data_t current)
    {
        return (previous & ~current & 0x0F) != 0;
    }

    export class joypad
    {
    public:
        void set_source(const joypad_source new_source) { source = new_source; }
        [[nodiscard]] joypad_source get_source() const { return source; }

        [[nodiscard]] const_input_state_view_t get_state() const { return state; }

        void set_state(const const_input_state_view_t new_input_state)
        {
            const auto prev_state = read_joypad_register(*this);
            std::ranges::copy(new_input_state, state.begin());
            const auto current_state = read_joypad_register(*this);

            if (detect_input_bits_falling_edge(prev_state, read_joypad_register(*this)))
            {
                using namespace interrupts;
                request<joypad_interrupt>(*memory);
            }
        }

        void connect(memory::memory_bus& bus) { memory = &bus; }

    private:
        joypad_source source{ joypad_source::none };
        std::array<bool, num_joypad_inputs> state {};

        memory::memory_bus* memory { nullptr };
    };

}
