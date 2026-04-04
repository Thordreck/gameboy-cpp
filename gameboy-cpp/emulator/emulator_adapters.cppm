
export module emulator:adapters;
export import emulator.core;
export import emulator.ui;
export import std;

import sdl;

namespace emulator
{
    export class engine_ui_adapter
    {
    public:
        explicit engine_ui_adapter(engine& engine)
            : engine { engine }
        {}

        // TODO: rethink this interface
        void load_rom(const rom_data_view_t data) { engine.load_rom(data); }
        void reset() { engine.reset(); }

        [[nodiscard]] framebuffer_view_t framebuffer() { return engine.lcd(); }

    private:
        engine& engine;
    };

    export class engine_input_source
    {
    public:
        explicit engine_input_source(engine& engine)
            : engine { engine }
        {}

        void update() const
        {
            using namespace sdl;
            using enum scancode;
            const keyboard_state keyboard = get_keyboard_state();

            joypad_input_state& input = engine.joypad_state();

            input.a      = keyboard.is_pressed(x).value_or(false);
            input.b      = keyboard.is_pressed(z).value_or(false);
            input.start  = keyboard.is_pressed(enter).value_or(false);
            input.select = keyboard.is_pressed(right_shift).value_or(false);
            input.up     = keyboard.is_pressed(up).value_or(false);
            input.down   = keyboard.is_pressed(down).value_or(false);
            input.left   = keyboard.is_pressed(left).value_or(false);
            input.right  = keyboard.is_pressed(right).value_or(false);
        }

    private:
        engine& engine;
    };

}