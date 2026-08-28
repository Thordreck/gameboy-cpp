
export module emulator.gameboy;
export import :factory;

import std;
import mbc;
import cartridge;
import emulator.core;
import emulator.engine;

namespace emulator
{
    export template<
        JoypadSource Joypad,
        FramebufferRenderer FramebufferRenderer,
        AudioOutputDevice<float> Audio,
        serial::SerialInterface Serial>
    class gameboy
    {
    public:
        explicit gameboy(Joypad& joypad_source, FramebufferRenderer& framebuffer_renderer, Audio& audio_output, Serial& serial)
            : joypad_source { joypad_source }
            , audio_output { audio_output }
            , framebuffer_renderer { framebuffer_renderer }
            , serial { serial }
        {}

        ~gameboy() { stop(); }

        [[nodiscard]] load_rom_result_t load_rom(cartridge::rom rom_cartridge)
        {
            stop();

            rom = std::move(rom_cartridge);
            auto create = create_engine(rom.value(), framebuffer_renderer, audio_output, serial);

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
        [[nodiscard]] memory_view memory() const { return engine->memory(); }
        [[nodiscard]] bool is_running() const { return tick_thread.has_value(); }
        [[nodiscard]] bool has_rom() const { return engine != nullptr; }
        [[nodiscard]] framebuffer_view_t framebuffer() const { return engine->lcd(); }

        void resume()
        {
            if (!is_running() && has_rom())
            {
                audio_output.open();
                framebuffer_renderer.start_rendering_frames();

                tick_thread = std::jthread { [this] (const auto& ct) { engine_tick_thread(*engine, ct); } };
                joypad_thread = std::jthread { [this] (const auto& ct) { engine_joypad_thread(*engine, joypad_source, ct); } };
            }
        }

        void pause()
        {
            tick_thread.reset();
            joypad_thread.reset();
            audio_output.close();
        }

        void stop()
        {
            pause();

            framebuffer_renderer.stop_rendering_frames();
            engine.reset();
            rom.reset();
        }

        void step(const std::uint32_t ticks)
        {
            engine->tick(ticks);
        }

    private:
        std::unique_ptr<base_engine> engine { nullptr };
        std::optional<cartridge::rom> rom {};

        Joypad& joypad_source;
        Audio& audio_output;
        FramebufferRenderer& framebuffer_renderer;
        Serial& serial;

        std::optional<std::jthread> tick_thread {};
        std::optional<std::jthread> joypad_thread {};
    };

}