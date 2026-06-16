export module interrupts:memory;

import memory;

import :common;
import :control;

namespace interrupts
{
    export [[nodiscard]] memory::memory_data_t read_ie_register(const interrupt_controller& control)
    {
        return (control.is_enabled(joypad_interrupt) << 4)
            | (control.is_enabled(serial_interrupt) << 3)
            | (control.is_enabled(timer_interrupt) << 2)
            | (control.is_enabled(lcd_interrupt) << 1)
            | (control.is_enabled(vblank_interrupt) << 0);
    }

    export void write_ie_register(interrupt_controller& control, const memory::memory_data_t data)
    {
        control.set_enabled(joypad_interrupt, data & joypad_interrupt.ie_flag());
        control.set_enabled(serial_interrupt, data & serial_interrupt.ie_flag());
        control.set_enabled(timer_interrupt, data & timer_interrupt.ie_flag());
        control.set_enabled(lcd_interrupt, data & lcd_interrupt.ie_flag());
        control.set_enabled(vblank_interrupt, data & vblank_interrupt.ie_flag());
    }

    export [[nodiscard]] memory::memory_data_t read_if_register(const interrupt_controller& control)
    {
        return (control.is_requested(joypad_interrupt) << 4)
            | (control.is_requested(serial_interrupt) << 3)
            | (control.is_requested(timer_interrupt) << 2)
            | (control.is_requested(lcd_interrupt) << 1)
            | (control.is_requested(vblank_interrupt) << 0)
            | 0xE0;
    }

    export void write_if_register(interrupt_controller& control, const memory::memory_data_t data)
    {
        control.set_requested(joypad_interrupt, data & joypad_interrupt.if_flag());
        control.set_requested(serial_interrupt, data & serial_interrupt.if_flag());
        control.set_requested(timer_interrupt, data & timer_interrupt.if_flag());
        control.set_requested(lcd_interrupt, data & lcd_interrupt.if_flag());
        control.set_requested(vblank_interrupt, data & vblank_interrupt.if_flag());
    }

}