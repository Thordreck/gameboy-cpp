module;
#include "emulator_ui_qt.hpp"

export module emulator.ui:qt;

import qt;
import utilities;
import cartridge;
import emulator.core;

namespace emulator
{
    template <Emulator Emulator>
    class emulator_ui_backend_adapter
    {
    public:
        explicit emulator_ui_backend_adapter(Emulator& emulator)
            : emulator { emulator }
        {}

        [[nodiscard]] bool has_rom() const { return emulator.has_rom(); }
        [[nodiscard]] bool is_running() const { return emulator.is_running(); }
        [[nodiscard]] ui_load_rom_result_t load_rom(const std::string_view path)
        {
            return cartridge::load_rom_file(path)
                .and_then([this] (const auto& data) { return emulator.load_rom(data); });
        }

        void resume() { emulator.resume(); }
        void pause() { emulator.pause(); }
        void stop() { emulator.stop(); }
        void step(const std::uint32_t ticks) { emulator.step(ticks); }

        [[nodiscard]] std::uint8_t read_memory(const std::uint16_t address) { return emulator.memory()[address]; }

        [[nodiscard]] float volume() const { return emulator.volume(); }
        void set_volume(const float volume) { emulator.set_volume(volume); }

        [[nodiscard]] bool muted() const { return emulator.muted(); }
        void set_muted(const bool muted) { emulator.set_muted(muted); }

    private:
        Emulator& emulator;
    };

    export class graphical_interface
    {
    public:
        graphical_interface(int& argc, char** argv)
            : app{argc, argv}
        {}

        void start_rendering_frames()
        {
            //framebuffer_source.start();
        }

        void stop_rendering_frames()
        {
            //framebuffer_source.stop();
        }

        void write_frame(const framebuffer_view_t frame)
        {
            framebuffer_source.push_frame(frame);
        }

        template <Emulator Imp>
        int render(Imp& emulator)
        {
            utils::panic_on_error(app.set_window_icon(":/icons/gameboy-icon.png"));

            emulator_ui_backend_adapter ui_adapter { emulator };
            emulator_ui_controls ui_controls { ui_adapter };

#ifdef QT_UI_DEBUG_MODE
            emulator_ui_sprites_model ui_debug_sprites_model { ui_adapter };
            emulator_ui_sprites_image_provider ui_debug_sprites_provider { ui_adapter };
            emulator_ui_background ui_debug_background { ui_adapter };
            emulator_ui_background_image_provider ui_debug_background_provider { ui_debug_background };

            engine.add_image_provider("sprites", &ui_debug_sprites_provider);
            engine.add_image_provider("background", &ui_debug_background_provider);

            qt::register_shortcut(qt::standard_key::refresh, [this]
            {
                std::ranges::for_each(engine.root_objects(), [] (auto object) { object.delete_later(); });
                engine.clear_singletons();
                engine.clear_component_cache();

                engine.load(QML_HOT_RELOAD_PATH);

            }, qt::shortcut_context::application, app);
#endif

            engine.set_initial_properties(
                std::make_pair("controls", &ui_controls),
                std::make_pair("framebuffer", &framebuffer_source),
#ifdef QT_UI_DEBUG_MODE
                std::make_pair("debugMode", true),
                std::make_pair("sprites", &ui_debug_sprites_model),
                std::make_pair("bg", &ui_debug_background));
#else
                std::make_pair("debugMode", false));
#endif

            engine.load_from_module("Gameboy.UI", "EmulatorUI");
            return app.execute();
        }

    private:
        qt::gui_application app;
        qt::qml_engine engine;

        emulator_ui_framebuffer_source framebuffer_source {};
    };
}