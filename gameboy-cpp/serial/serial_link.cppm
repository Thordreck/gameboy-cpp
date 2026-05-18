module;
#include "profiling.hpp"

export module serial:link;

import std;
import :common;

namespace serial
{
    constexpr std::uint16_t serial_master_clock_steps { 512 };

    export class link
    {
    public:
        [[nodiscard]] std::uint8_t get_transfer_data() const { return transfer_data; }
        void set_transfer_data(const std::uint8_t new_data) { transfer_data = new_data; }

        [[nodiscard]] bool is_transfer_enabled() const { return transfer_enabled; }
        void set_transfer_enabled(const bool enabled) { transfer_enabled = enabled; }

        [[nodiscard]] bool is_master() const { return master; }
        void set_master(const bool enabled) { master = enabled; }

        [[nodiscard]] bool active() const { return is_transfer_enabled() && is_master(); }
        [[nodiscard]] std::uint32_t tick_batch() const
        {
            if (!active()) [[unlikely]]
            {
                return std::numeric_limits<std::uint32_t>::max();
            }

            return (8 - num_bits_transferred) * serial_master_clock_steps;
        }

        template<SerialInterface Interface>
        void tick(const std::uint32_t num_ticks, Interface& imp)
        {
            PROFILER_SCOPE("Serial::tick()");

            if (!active()) [[unlikely]] { return; }

            cycle_accumulator += num_ticks;

            while (cycle_accumulator >= serial_master_clock_steps)
            {
                cycle_accumulator -= serial_master_clock_steps;
                transfer_bit(imp);
            }
        }

        template<SerialInterface Interface>
        void external_tick(Interface& imp)
        {
            PROFILER_SCOPE("Serial::external_tick()");

            if (transfer_enabled && !master)
            {
                transfer_bit(imp);
            }
        }

    private:
        template<SerialInterface Interface>
        void transfer_bit(Interface& imp)
        {
            const std::uint8_t sent_bit = transfer_data >> 7;
            const std::uint8_t received_bit = imp.transfer_bit(sent_bit);

            transfer_data = transfer_data << 1 | (received_bit & 0b1);
            num_bits_transferred++;

            if (num_bits_transferred >= 8)
            {
                // TODO: interrupt
                transfer_enabled = false;
                num_bits_transferred = 0;
            }
        }

        std::uint8_t transfer_data {};
        bool transfer_enabled { false };
        bool master { false };
        std::uint8_t num_bits_transferred { 0 };
        std::uint32_t cycle_accumulator { 0 };
    };

}