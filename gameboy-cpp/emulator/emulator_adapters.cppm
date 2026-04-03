
export module emulator:adapters;
export import emulator.core;
export import emulator.ui;
export import std;

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

}