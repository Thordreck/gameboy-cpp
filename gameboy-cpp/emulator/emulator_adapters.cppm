
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
            : sink { engine.joypad() }
            , source { sdl::get_keyboard_state() }
        {}

        void update() const
        {
            using enum sdl::scancode;

            sink.set_input_state(
                source[enter],
                source[right_shift],
                source[up],
                source[down],
                source[left],
                source[right],
                source[x],
                source[z]);
        }

    private:
        joypad_input_sink sink;
        sdl::keyboard_state source;
    };

}