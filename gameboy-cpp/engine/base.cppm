
export module engine:base;

import std;
import joypad;
import graphics;

import :common;

namespace engine
{
    export class base_engine
    {
    public:
        virtual ~base_engine() = default;

        [[nodiscard]] virtual lcd_view_t lcd() const = 0;
        [[nodiscard]] virtual memory_view memory() const = 0;

        virtual void update_joypad_state(joypad::const_input_state_view_t state) = 0;
        virtual void tick(std::uint32_t num_ticks) = 0;
        virtual void tick_external_serial_clock() = 0;
    };

    export using base_engine_ptr = std::unique_ptr<base_engine>;

}