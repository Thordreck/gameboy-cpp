
module;
#include "profiling.hpp"

export module emulator.ui:sdl;

import std;
import sdl;
import sdl_image;
import pfd;
import imgui;
import cartridge;
import utilities;
import emulator.core;

namespace emulator
{
    export class graphical_interface
    {
    public:
        graphical_interface(int, char**)
            : sdl_session { utils::value_or_panic(sdl::session::create(sdl::init_flags::video) )}
            , sdl_window { utils::value_or_panic(sdl::window::create("gameboy-cpp", 640, 480, sdl::window_flags::resizable)) }
            , sdl_renderer { utils::value_or_panic(sdl::renderer::create(sdl_window)) }
            , sdl_texture { utils::value_or_panic(sdl::texture::create(sdl_renderer, sdl::pixel_format::rgb_24, sdl::texture_access::streaming, 160, 144)) }
            , imgui_context { utils::value_or_panic(imgui::context::create()) }
            , imgui_backend { utils::value_or_panic(imgui::sdl3_renderer_backend::create(sdl_renderer)) }
            , imgui_platform { utils::value_or_panic(imgui::sdl3_platform::with_renderer(sdl_window, sdl_renderer)) }
            , window_icon { utils::value_or_panic(sdl::load_image(std::filesystem::current_path() / "assets" / "icons" / "gameboy-icon.png"))}
        {
            imgui::check_version();
            imgui::set_dark_style();

            utils::panic_on_error(sdl_window.set_icon(window_icon));
            utils::panic_on_error(sdl_renderer.set_vsync(sdl::renderer_vsync::disabled));
        }

        template <Emulator Imp>
        int render(Imp& emulator)
        {
            while (!should_quit)
            {
                PROFILER_SCOPE("UI Frame");

                using namespace std::chrono_literals;
                utils::execute_for(
                    [this, &emulator] { render_once(emulator); }
                    , 16ms
                    , [](const auto& d) { std::this_thread::sleep_for(d); });
            }

            return 0;
        }

    private:
        template <Emulator Imp>
        void render_once(Imp& emulator)
        {
            PROFILER_SCOPE("Engine Thread");

            while (const auto event_opt = sdl::poll_event())
            {
                const auto event = event_opt.value();
                std::ignore = imgui_platform.process_event(event);

                should_quit = std::holds_alternative<sdl::quit_event>(event.parse());
                if (should_quit) { return; }
            }

            imgui_backend.new_frame();
            imgui_platform.new_frame();
            imgui::new_frame();

            // Main menu
            if (const auto main_menu = imgui::main_menu_bar(); main_menu)
            {
                if (const auto file_menu = imgui::menu("File"); file_menu)
                {
                    if (imgui::menu_item("Open Rom"))
                    {
                        constexpr std::array rom_filter { pfd::filter { "Rom Files", "*.gb" } };

                        if (const auto rom_selection = pfd::open_file("Select Rom", std::nullopt, rom_filter); rom_selection.has_value())
                        {
                            const auto load_result
                                = cartridge::load_rom_file(rom_selection.value())
                                .and_then([&emulator] (const auto& data) { return emulator.load_rom(data); });

                            if (!load_result.has_value())
                            {
                                pfd::error_dialog("Error loading rom", load_result.error());
                            }
                        }
                    }

                    if (emulator.is_running() && imgui::menu_item("Display Rom Details"))
                    {
                        pfd::message_dialog("Rom Details", cartridge::pretty_print(emulator.cartridge()));
                    }

                    if (emulator.is_running() && imgui::menu_item("Pause"))
                    {
                        emulator.pause();
                    }

                    if (emulator.is_running() && imgui::menu_item("Stop"))
                    {
                        emulator.stop();
                    }

                    if (!emulator.is_running() && imgui::menu_item("Resume"))
                    {
                        emulator.resume();
                    }
                }
            }

            // Viewport
            if (const auto viewport = imgui::window("Viewport"); viewport && emulator.has_rom())
            {
                auto [lock, surface] = utils::value_or_panic(sdl::lock_to_surface(sdl_texture));
                std::ranges::copy(emulator.framebuffer(), static_cast<std::uint8_t*>(surface.pixels()));

                const imgui::vec2 viewport_size = imgui::get_available_content_space();
                const imgui::texture_id texture_id = reinterpret_cast<imgui::texture_id>(sdl::internal::native::get_handle(sdl_texture));
                imgui::image(texture_id, viewport_size);
            }

            imgui::render();
            utils::panic_on_error(sdl_renderer.clear());
            imgui_backend.render();
            utils::panic_on_error(sdl_renderer.present());
        }

        sdl::session sdl_session;
        sdl::window sdl_window;
        sdl::renderer sdl_renderer;
        sdl::texture sdl_texture;

        imgui::context imgui_context;
        imgui::sdl3_renderer_backend imgui_backend;
        imgui::sdl3_platform imgui_platform;

        sdl::surface window_icon;

        bool should_quit { false };
    };

}
