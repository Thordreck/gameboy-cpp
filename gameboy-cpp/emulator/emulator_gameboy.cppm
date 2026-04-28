
export module emulator.gameboy;

import std;
import mbc;
import :factory;
import cartridge;
import emulator.core;
import emulator.engine;

namespace emulator
{
    export template<JoypadSource Joypad>
    class gameboy
    {
    public:
        explicit gameboy(Joypad& joypad_source)
            : joypad_source { joypad_source }
        {}

        ~gameboy() { stop(); }

        [[nodiscard]] load_rom_result_t load_rom(cartridge::rom rom_cartridge)
        {
            stop();

            rom = std::move(rom_cartridge);
            auto create = create_engine(rom.value());

            if (create)
            {
                engine = std::move(create.value());
                resume();

                return {};
            }

            rom.reset();
            return std::unexpected { create.error() };
        }

        [[nodiscard]] cartridge::header cartridge() const { return rom.value().header; }
        [[nodiscard]] framebuffer_view_t framebuffer() const { return engine->lcd(); }
        [[nodiscard]] bool is_running() const { return tick_thread.has_value(); }
        [[nodiscard]] bool has_rom() const { return engine != nullptr; }

        void resume()
        {
            if (!is_running() && has_rom())
            {
                tick_thread = std::jthread { [this] (const auto& ct) { engine_tick_thread(*engine, ct); } };
                joypad_thread = std::jthread { [this] (const auto& ct) { engine_joypad_thread(*engine, joypad_source, ct); } };
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
            rom.reset();
        }

    private:
        std::unique_ptr<base_engine> engine { nullptr };
        std::optional<std::jthread> tick_thread {};
        std::optional<std::jthread> joypad_thread {};
        std::optional<cartridge::rom> rom {};

        Joypad& joypad_source;
    };

}