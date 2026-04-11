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
            if (overflow_detected)
            {
                return ticks_until_interrupt;
            }

            if (!timer_control.enabled)
            {
                return std::numeric_limits<std::uint32_t>::max();
            }

            return number_of_ticks_for_tima_increment(divider_register, timer_control.clock);
        }

        void tick(const std::uint32_t num_ticks)
        {
            PROFILER_SCOPE("Timer System::tick()");

            std::uint32_t remaining_ticks = num_ticks;

            while (remaining_ticks > 0)
            {
                const std::uint32_t batch = std::min(remaining_ticks, tick_batch());
                advance(batch);

                remaining_ticks -= batch;
            }
        }

        void cancel_pending_interrupt()
        {
            overflow_detected = false;
            ticks_until_interrupt = 0;
        }

        void connect(memory::memory_bus& bus) { memory = &bus; }

    private:
        void advance(const std::uint32_t tick_batch)
        {
            if (overflow_detected)
            {
                ticks_until_interrupt -= std::min(tick_batch, static_cast<std::uint32_t>(ticks_until_interrupt));

                if (ticks_until_interrupt == 0)
                {
                    timer_counter = timer_modulo.value;
                    overflow_detected = false;

                    using namespace interrupts;
                    request<timer_interrupt>(*memory);
                }

                divider_register = divider_register + tick_batch;
                return;
            }

            if (!timer_control.enabled)
            {
                divider_register = divider_register + tick_batch;
                return;
            }

            const std::uint8_t bit = get_tack_clock_bit_index(timer_control.clock);
            const std::uint16_t mask = (1u << (bit + 1)) - 1;

            const std::uint16_t prev_div = divider_register;
            const std::uint16_t lower = prev_div & mask;
            const bool falling_edge = (lower + tick_batch) > mask;

            divider_register = divider_register + tick_batch;

            // Tima
            if (falling_edge)
            {
                overflow_detected = timer_counter.tick();
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
