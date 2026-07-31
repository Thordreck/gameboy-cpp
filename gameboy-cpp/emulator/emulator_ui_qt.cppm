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
        [[nodiscard]] ui_framebuffer_t framebuffer() { return emulator.framebuffer(); }
        [[nodiscard]] ui_load_rom_result_t load_rom(const std::string_view path)
        {
            return cartridge::load_rom_file(path)
                .and_then([this] (const auto& data) { return emulator.load_rom(data); });
        }

        void resume() { emulator.resume(); }
        void pause() { emulator.pause(); }
        void stop() { emulator.stop(); }

    private:
        Emulator& emulator;
    };

    export class graphical_interface
    {
    public:
        graphical_interface(int& argc, char** argv)
            : app{argc, argv}
        {
            using namespace qt;

        }

        template <Emulator Imp>
        int render(Imp& emulator)
        {
            utils::panic_on_error(app.set_window_icon(":/icons/gameboy-icon.png"));

            emulator_ui_backend_adapter ui_adapter { emulator };
            emulator_ui_backend ui_backend { ui_adapter };
            emulator_ui_backend_singleton::set_backend(&ui_backend);

#ifdef QML_HOT_RELOAD
            qt::register_shortcut(qt::standard_key::refresh, [this]
            {
                std::println("Hot-reloading qml file from path: {}", QML_HOT_RELOAD_PATH);

                std::ranges::for_each(engine.root_objects(), [] (auto object) { object.delete_later(); });
                engine.clear_singletons();
                engine.clear_component_cache();

                engine.load(QML_HOT_RELOAD_PATH);

            }, qt::shortcut_context::application, app);
#endif

            engine.load_from_module("Gameboy.UI", "EmulatorUI");
            return app.execute();
        }

    private:
        qt::gui_application app;
        qt::qml_engine engine;
    };
}