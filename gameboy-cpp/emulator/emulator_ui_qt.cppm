module;
#include "emulator_ui_qt.hpp"

export module emulator:ui.qt;

import utilities;
import cartridge;
import :common;

namespace emulator
{
    export class graphical_interface
    {
    public:
        graphical_interface(const int argc, char** argv)
            : imp { argc, argv }
        {}

        void start_rendering_frames()
        {
            imp.start_rendering();
        }

        void stop_rendering_frames()
        {
            imp.stop_rendering();
        }

        void write_frame(const ui_framebuffer_view_t frame)
        {
            imp.present_frame(frame);
        }

        template <Emulator Imp>
        int render(Imp& emulator)
        {
            const backend_functions backend
            {
                [&emulator] { return emulator.has_rom(); },
                [&emulator] { return emulator.is_running(); },
                [&emulator] (const auto& path)
                {
                    return cartridge::load_rom_file(path)
                        .and_then([&emulator] (const auto& data) { return emulator.load_rom(data); });
                },
                [&emulator] { return emulator.resume(); },
                [&emulator] { return emulator.pause(); },
                [&emulator] { return emulator.stop(); },
                [&emulator] (const auto step ){ return emulator.step(step); },

                [&emulator] (const auto address){ return emulator.memory()[address]; },

                [&emulator] { return emulator.volume(); },
                [&emulator] (const float volume) { return emulator.set_volume(volume); },

                [&emulator] { return emulator.muted(); },
                [&emulator] (const bool muted) { return emulator.set_muted(muted); },
            };

            return imp.render(backend);
            //utils::panic_on_error(app.set_window_icon(":/icons/gameboy-icon.png"));

            //emulator_ui_controls ui_controls { ui_adapter };

#ifdef QT_UI_DEBUG_MODE
            //emulator_ui_sprites_model ui_debug_sprites_model { ui_adapter };
            //emulator_ui_sprites_image_provider ui_debug_sprites_provider { ui_adapter };
            //emulator_ui_background ui_debug_background { ui_adapter };
            //emulator_ui_background_image_provider ui_debug_background_provider { ui_debug_background };

            //engine.add_image_provider("sprites", &ui_debug_sprites_provider);
            //engine.add_image_provider("background", &ui_debug_background_provider);

            /*
            qt::register_shortcut(qt::standard_key::refresh, [this]
            {
                std::ranges::for_each(engine.root_objects(), [] (auto object) { object.delete_later(); });
                engine.clear_singletons();
                engine.clear_component_cache();

                engine.load(QML_HOT_RELOAD_PATH);

            }, qt::shortcut_context::application, app);
            */
#endif

            /*
            engine.set_initial_properties(
                ////std::make_pair("controls", &ui_controls),
                //std::make_pair("framebuffer", &framebuffer_source),
#ifdef QT_UI_DEBUG_MODE
                std::make_pair("debugMode", true));
                //std::make_pair("sprites", &ui_debug_sprites_model),
                //std::make_pair("bg", &ui_debug_background));
#else
                std::make_pair("debugMode", false));
#endif

            engine.load_from_module("Gameboy.UI", "EmulatorUI");
            return app.execute();
            */
        }

    private:
        qt_graphical_interface imp;
    };
}