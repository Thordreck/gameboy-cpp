module;
#include "profiling.hpp"

export module timer:timer_system;

import std;
import interrupts;

import :div;
import :tima;
import :tma;
import :tac;

namespace timer
{
    static std::uint32_t number_of_ticks_for_tima_increment(
        const std::uint16_t div,
        const tac_clock selected_clock)
    {
        const std::uint8_t bit_index = get_tack_clock_bit_index(selected_clock);
        const std::uint32_t mask = (1u << (bit_index + 1)) - 1;
        const std::uint32_t lower = div & mask;

        return (mask - lower) + 1;
    }

    static std::uint32_t number_of_ticks_for_div_increment(const std::uint16_t div)
    {
        return std::numeric_limits<std::uint8_t>::max() - static_cast<std::uint8_t>(div & 0xFF) + 1;
    }

    static std::uint32_t number_of_ticks_for_div_clock_bit_set(const std::uint16_t div, const tac_clock selected_clock)
    {
        const std::uint8_t bit_index = get_tack_clock_bit_index(selected_clock);
        const std::uint32_t mask = (1u << bit_index) - 1;
        const std::uint32_t lower = div & mask;
        const std::uint32_t raw_num_ticks = mask - lower + 1;

        return std::min(1u, raw_num_ticks);
    }

    export class timer_system
    {
    public:
        [[nodiscard]] const div& divider() const { return divider_register; }

        [[nodiscard]] std::uint16_t get_divider() const { return divider_register.value(); }

        void set_divider(const std::uint16_t value)
        {
            const std::uint16_t prev_div = divider_register;
            divider_register = value;

            if (timer_control.enabled)
            {
                const std::uint16_t mask = 0b1 << get_tack_clock_bit_index(timer_control.clock);
                const bool falling_edge = (prev_div & mask) > (divider_register.value() & mask);

                if(falling_edge)
                {
                    overflow_detected = timer_counter.tick();
                    ticks_until_interrupt = overflow_detected ? 4 : 0;
                }
            }
        }

        [[nodiscard]] std::uint8_t get_counter() const
        {
            return timer_counter.value();
        }

        void set_counter(const std::uint8_t value)
        {
            if (overflow_detected && ticks_until_interrupt <= 4)
            {
                timer_counter = value;
                overflow_detected = false;
                ticks_until_interrupt = 0;
            }
            else if (!tma_overwrite_performed_this_cycle)
            {
                timer_counter = value;
            }
        }

        [[nodiscard]] std::uint8_t get_modulo() const { return timer_modulo.value; }
        void set_modulo(const std::uint8_t value)
        {
            timer_modulo.value = value;

            if (tma_overwrite_performed_this_cycle)
            {
                timer_counter = value;
            }
        }

        [[nodiscard]] bool is_control_enabled() const { return timer_control.enabled; }

        template<interrupts::InterruptRequestController InterruptController>
        void set_control_enabled(const bool value, InterruptController& interrupts)
        {
            const bool was_enabled = timer_control.enabled;
            timer_control.enabled = value;

            if (was_enabled && !value)
            {
                const std::uint16_t mask = 0b1 << get_tack_clock_bit_index(timer_control.clock);
                const bool clock_bit_high = divider_register.value() & mask;

                if (clock_bit_high && timer_counter.tick())
                {
                    interrupts.request(interrupts::timer_interrupt);
                }
            }
        }

        [[nodiscard]] tac_clock get_control_clock() const { return timer_control.clock; }
        void set_control_clock(const tac_clock value)
        {
            const auto prev_clock = timer_control.clock;
            timer_control.clock = value;

            if (timer_control.enabled)
            {
                const std::uint16_t prev_mask = 0b1 << get_tack_clock_bit_index(prev_clock);
                const std::uint16_t current_mask = 0b1 << get_tack_clock_bit_index(timer_control.clock);
                const bool falling_edge = (divider_register.value() & prev_mask) > (divider_register.value() & current_mask);

                if(falling_edge)
                {
                    overflow_detected = timer_counter.tick();
                    ticks_until_interrupt = overflow_detected ? 4 : 0;
                }
            }
        }

        [[nodiscard]] bool active() const { return true; }
        [[nodiscard]] std::uint32_t tick_batch() const
        {
            const std::uint32_t ticks_until_interrupt_triggered = overflow_detected
                ? ticks_until_interrupt
                : std::numeric_limits<std::uint32_t>::max();

            const std::uint32_t ticks_until_div_increment = number_of_ticks_for_div_increment(divider_register.value());
            const std::uint32_t ticks_until_div_bit_set = number_of_ticks_for_div_clock_bit_set(divider_register.value(), timer_control.clock);

            const std::uint32_t ticks_until_tima_increment = timer_control.enabled
                ? number_of_ticks_for_tima_increment(divider_register, timer_control.clock)
                : std::numeric_limits<std::uint32_t>::max();

            return std::min(
                std::min(ticks_until_interrupt_triggered, ticks_until_div_bit_set),
                std::min(ticks_until_div_increment, ticks_until_tima_increment));
        }

        template<interrupts::InterruptRequestController InterruptController>
        void tick(const std::uint32_t num_ticks, InterruptController& interrupts)
        {
            PROFILER_SCOPE("Timer System::tick()");

            std::uint32_t remaining_ticks = num_ticks;

            while (remaining_ticks > 0)
            {
                const std::uint32_t batch = std::min(remaining_ticks, tick_batch());
                advance(batch, interrupts);

                remaining_ticks -= batch;
            }
        }

    private:
        template<interrupts::InterruptRequestController InterruptController>
        void advance(const std::uint32_t tick_batch, InterruptController& interrupts)
        {
            remaining_ticks_in_tma_overwrite_cycle -= std::min(static_cast<std::uint32_t>(remaining_ticks_in_tma_overwrite_cycle), tick_batch);
            tma_overwrite_performed_this_cycle = remaining_ticks_in_tma_overwrite_cycle > 0;

            if (overflow_detected)
            {
                ticks_until_interrupt -= std::min(tick_batch, static_cast<std::uint32_t>(ticks_until_interrupt));

                if (ticks_until_interrupt == 0)
                {
                    timer_counter = timer_modulo.value;
                    overflow_detected = false;

                    tma_overwrite_performed_this_cycle = true;
                    remaining_ticks_in_tma_overwrite_cycle = 4;

                    interrupts.request(interrupts::timer_interrupt);
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

        bool tma_overwrite_performed_this_cycle { false };
        std::uint8_t remaining_ticks_in_tma_overwrite_cycle { 0 };
    };
}
