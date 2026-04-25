
export module emulator.gameboy;

import std;
import cartridge;
import emulator.core;

namespace emulator
{
    export template<Engine Engine, JoypadSource Joypad>
    class gameboy
    {
    public:
        explicit gameboy(Joypad& joypad_source)
            : joypad_source { joypad_source }
        {}

        ~gameboy() { stop(); }

        [[nodiscard]] load_rom_result_t load_rom(const cartridge::rom& rom)
        {
            stop();
            engine.emplace(rom);
            resume();

            return {};
        }

        [[nodiscard]] cartridge::header cartridge() const { return engine.value().cartridge(); }
        [[nodiscard]] framebuffer_view_t framebuffer() const { return engine.value().lcd(); }
        [[nodiscard]] bool is_running() const { return tick_thread.has_value(); }
        [[nodiscard]] bool has_rom() const { return engine.has_value(); }

        void resume()
        {
            if (!is_running() && engine.has_value())
            {
                tick_thread = std::jthread { [this] (const auto& ct) { engine_tick_thread(engine.value(), ct); } };
                joypad_thread = std::jthread { [this] (const auto& ct) { engine_joypad_thread(engine.value(), joypad_source, ct); } };
            }
        }

        void pause()
        {
            tick_thread.reset();
            joypad_thread.reset();
        }

        void stop()
        {
            pause();
            engine.reset();
        }

    private:
        std::optional<Engine> engine {};
        std::optional<std::jthread> tick_thread {};
        std::optional<std::jthread> joypad_thread {};

        Joypad& joypad_source;
    };

}