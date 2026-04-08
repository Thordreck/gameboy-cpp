module;
#include "profiling.hpp"

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
        return (previous & ~current & 0x0F) != 0;
    }

    export class joypad_system
    {
    public:
        void tick()
        {
            PROFILER_SCOPE("Joypad System");
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