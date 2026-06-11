export module timer:memory;

import std;
import memory;
import interrupts;

import :common;
import :timer_system;

namespace timer
{
    export constexpr memory::memory_address_t div_address = 0xFF04;
    export constexpr memory::memory_address_t tima_address = 0xFF05;
    export constexpr memory::memory_address_t tma_address = 0xFF06;
    export constexpr memory::memory_address_t tac_address = 0xFF07;

    constexpr std::uint8_t tac_enable_bit = 2;

    tac_clock to_clock_select(const std::uint8_t value)
    {
        constexpr std::uint8_t tac_clock_select_mask = 0b11;
        const std::uint8_t clock_value = value & tac_clock_select_mask;

        using enum tac_clock;

        switch (clock_value)
        {
        case 0b00: return _00;
        case 0b01: return _01;
        case 0b10: return _10;
        case 0b11: return _11;
        default: std::unreachable();
        }
    }

    std::uint8_t to_register_value(const tac_clock clock_value)
    {
        using enum tac_clock;

        switch (clock_value)
        {
        case _00: return 0b00;
        case _01: return 0b01;
        case _10: return 0b10;
        case _11: return 0b11;
        default: std::unreachable();
        }
    }

    export [[nodiscard]] memory::memory_data_t read_div_address(const timer_system& timer)
    {
        return timer.get_divider() >> 8;
    }

    export void write_div_address(timer_system& timer, const memory::memory_data_t)
    {
        timer.set_divider(0x00);
    }

    export [[nodiscard]] memory::memory_data_t read_tima_address(const timer_system& timer)
    {
        return timer.get_counter();
    }

    export void write_tima_address(timer_system& timer, const memory::memory_data_t data)
    {
        timer.set_counter(data);
    }

    export [[nodiscard]] memory::memory_data_t read_tma_address(const timer_system& timer)
    {
        return timer.get_modulo();
    }

    export void write_tma_address(timer_system& timer, const std::uint8_t value)
    {
        timer.set_modulo(value);
    }

    export [[nodiscard]] memory::memory_data_t read_tac_address(const timer_system& system)
    {
        return (system.is_control_enabled() << tac_enable_bit)
            | to_register_value(system.get_control_clock())
            | 0xF8;
    }

    export template<interrupts::InterruptRequestController InterruptController>
    void write_tac_address(timer_system& system, const memory::memory_data_t data, InterruptController& interrupts)
    {
        system.set_control_enabled(utils::is_bit_set<tac_enable_bit>(data), interrupts);
        system.set_control_clock(to_clock_select(data));
    }

}
