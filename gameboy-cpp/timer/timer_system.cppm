module;
#include "profiling.hpp"

export module timer:timer_system;

export import std;
export import memory;
export import interrupts;

export import :div;
export import :tima;
export import :tma;
export import :tac;

namespace timer
{
    static std::uint32_t number_of_ticks_for_tima_increment(
        const std::uint8_t div,
        const tac_clock selected_clock)
    {
        const std::uint8_t bit_index = get_tack_clock_bit_index(selected_clock);
        const std::uint32_t mask = (1u << (bit_index + 1)) - 1;
        const std::uint32_t lower = div & mask;

        return (mask - lower) + 1;
    }

    export class timer_system
    {
    public:
        div& divider() { return divider_register; }
        const div& divider() const { return divider_register; }

        tima& counter() { return timer_counter; }
        const tima& counter() const { return timer_counter; }

        tma& modulo() { return timer_modulo; }
        const tma& modulo() const { return timer_modulo; }

        tac& control() { return timer_control; }
        const tac& control() const { return timer_control; }

        [[nodiscard]] bool active() const { return true; }
        [[nodiscard]] std::uint32_t tick_batch() const
        {
            return overflow_detected
                       ? ticks_until_interrupt
                       : timer_control.enabled
                       ? number_of_ticks_for_tima_increment(divider_register, timer_control.clock)
                       : std::numeric_limits<std::uint32_t>::max();
        }

        void tick(const std::uint32_t num_ticks)
        {
            PROFILER_SCOPE("Timer System::tick()");

            for (std::uint32_t i = 0; i < num_ticks; ++i)
            {
                tick();
            }
        }

        void cancel_pending_interrupt()
        {
            overflow_detected = false;
            ticks_until_interrupt = 0;
        }

        void connect(memory::memory_bus& bus) { memory = &bus; }

    private:
        void tick()
        {
            if (overflow_detected && --ticks_until_interrupt <= 0)
            {
                timer_counter = timer_modulo.value;
                overflow_detected = false;

                using namespace interrupts;
                request<timer_interrupt>(*memory);
            }

            // Div
            const std::uint16_t prev_div = divider_register;
            divider_register.tick();
            const std::uint16_t current_div = divider_register;

            // Tima
            if (!overflow_detected && timer_control.enabled)
            {
                overflow_detected = timer_counter.tick(prev_div, current_div, timer_control.clock);
                ticks_until_interrupt = overflow_detected ? 4 : 0;
            }
        }

        div divider_register{};
        tima timer_counter{};
        tma timer_modulo{};
        tac timer_control{};

        bool overflow_detected{false};
        std::uint8_t ticks_until_interrupt{};

        memory::memory_bus* memory{nullptr};
    };
}
