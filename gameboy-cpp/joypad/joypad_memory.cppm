
export module joypad:memory;

import memory;
import :common;

namespace joypad
{
    export constexpr memory::memory_address_t joypad_memory_address = 0xFF00;

    [[nodiscard]] joypad_source get_source_from_register_value(const memory::memory_data_t value)
    {
        using enum joypad_source;
        const auto selection_bits = (value >> 4) & 0b11;

        switch (selection_bits)
        {
        case 0b00: return both;
        case 0b10: return d_pad;
        case 0b01: return buttons;
        case 0b11: return none;
        default: std::unreachable();
        }
    }

    [[nodiscard]] memory::memory_data_t get_register_value_from_source(const joypad_source source)
    {
        using enum joypad_source;

        switch (source)
        {
        case both: return 0b00;
        case d_pad: return 0b10;
        case buttons: return 0b01;
        case none: return 0b11;
        default: std::unreachable();
        }
    }

    template<joypad_input input>
    concept JoypadButtonInput
        = input == joypad_input::start
        || input == joypad_input::select
        || input == joypad_input::b
        || input == joypad_input::a;

    template<joypad_input input>
    concept JoypadDPadInput
        = input == joypad_input::down
        || input == joypad_input::up
        || input == joypad_input::left
        || input == joypad_input::right;

    template<joypad_input button_input, joypad_input d_pad_input>
    requires JoypadButtonInput<button_input> && JoypadDPadInput<d_pad_input>
    static bool is_multiplexed_input_pressed(const joypad_source source, const const_input_state_view_t state)
    {
        const bool select_buttons =
            source == joypad_source::buttons || source == joypad_source::both;

        const bool select_dpad =
            source == joypad_source::d_pad || source == joypad_source::both;

        bool pressed = false;

        if (select_buttons)
        {
            constexpr auto index = std::to_underlying(button_input);
            pressed |= state[index];
        }

        if (select_dpad)
        {
            constexpr auto index = std::to_underlying(d_pad_input);
            pressed |= state[index];
        }

        return pressed;
    }

    export template <JoypadWritableSource Joypad>
    void write_joypad_register(Joypad& joypad, const memory::memory_data_t input)
    {
        const joypad_source source = get_source_from_register_value(input);
        joypad.set_source(source);
    }

    export template <MultiplexedReadableJoypad Joypad>
    [[nodiscard]] memory::memory_data_t read_joypad_register(const Joypad& joypad)
    {
        using enum joypad_input;

        const joypad_source source = joypad.get_source();
        const auto state = joypad.get_state();

        return (get_register_value_from_source(source) << 4)
            | (!is_multiplexed_input_pressed<start, down>(source, state) << 3)
            | (!is_multiplexed_input_pressed<select, up>(source, state) << 2)
            | (!is_multiplexed_input_pressed<b, left>(source, state) << 1)
            | !is_multiplexed_input_pressed<a, right>(source, state);
    }

}