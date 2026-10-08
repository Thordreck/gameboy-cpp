
export module engine:common;

import std;
import joypad;
import graphics;

namespace engine
{
    export constexpr std::uint32_t num_ticks_per_frame { 70224 };
    export constexpr std::chrono::nanoseconds tick_duration{238 };
    export constexpr std::chrono::milliseconds frame_duration {
        std::chrono::duration_cast<std::chrono::milliseconds>(num_ticks_per_frame * tick_duration)
    };

    export template <typename T>
    concept Engine = requires(T& engine, const std::uint32_t num_ticks, const joypad::const_input_state_view_t joypad_state)
    {
        { engine.tick(num_ticks) } -> std::same_as<void>;
        { engine.update_joypad_state(joypad_state) } -> std::same_as<void>;
    };

    export using lcd_view_t = std::span<const memory::memory_data_t, graphics::lcd_memory_size>;

    export class memory_view
    {
    public:
        template<memory::ReadOnlyMemory Memory>
        explicit memory_view(const Memory& memory)
            : read_fn([&memory] (const auto address) { return memory.read(address); })
        {}

        [[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const { return read_fn(address); }
        [[nodiscard]] memory::memory_data_t operator[](const memory::memory_address_t address) const { return read(address); }

    private:
        std::function<memory::memory_data_t(memory::memory_address_t address)> read_fn;
    };

}