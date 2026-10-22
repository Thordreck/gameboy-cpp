module;
#include "emulator_ui_qt.hpp"

export module emulator:ui.qt;

import utilities;
import cartridge;
import metadata;

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
            using namespace utils;

            const auto third_party_modules = value_or_panic(meta::get_third_party_metadata());
            const auto project_meta = meta::get_project_metadata();

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

                [&project_meta]
                {
                    return project_metadata
                    {
                        project_meta.author,
                        project_meta.name,
                        project_meta.version,
                        project_meta.description,
                        project_meta.url,
                        project_meta.license,
                        project_meta.hash,
                    };
                },

                [&third_party_modules]
                {
                    return third_party_modules
                        | std::views::transform([] (auto&& entry) { return third_party_metadata { entry.name, entry.url, entry.license }; })
                        | std::ranges::to<std::vector>();
                }
            };

            return imp.render(backend);
        }

    private:
        qt_graphical_interface imp;
    };
}