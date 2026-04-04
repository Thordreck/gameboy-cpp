
export module joypad:system;

import :common;
import :memory;

import memory;
import utilities;
import interrupts;

namespace joypad
{
    static bool detect_input_bits_falling_edge(const memory::memory_data_t previous, const memory::memory_data_t current)
    {
        using namespace utils;

        return bit_falling_edge<3>(previous, current)
            || bit_falling_edge<2>(previous, current)
            || bit_falling_edge<1>(previous, current)
            || bit_falling_edge<0>(previous, current);
    }

    export class joypad_system
    {
    public:
        void tick()
        {
            utils::assert_not_nullptr(memory);

            const auto current_register_value = memory->read(joypad_memory_address);

            if (detect_input_bits_falling_edge(prev_register_value, current_register_value))
            {
                using namespace interrupts;
                request<joypad_interrupt>(*memory);
            }

            prev_register_value = current_register_value;
        }

        void connect(memory::memory_bus& bus) { memory = &bus; }

    private:
        memory::memory_data_t prev_register_value {};
        memory::memory_bus* memory { nullptr };
    };

}