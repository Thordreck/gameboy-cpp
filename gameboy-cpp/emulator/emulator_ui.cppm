module;
#include "profiling.hpp"

export module emulator.ui;

import std;
import sdl;
import pfd;
import imgui;
import utilities;

namespace emulator
{
    export constexpr std::size_t framebuffer_size = 160 * 144 * 3;
    export using framebuffer_view_t = std::span<const std::uint8_t, framebuffer_size>;

    export template <typename T>
    concept EmulatorUIInterface = requires(T& emulator, const std::vector<std::uint8_t>& rom_data)
    {
        { emulator.load_rom(rom_data) } -> std::same_as<void>;
        { emulator.reset() } -> std::same_as<void>;
        { emulator.framebuffer() } -> std::convertible_to<framebuffer_view_t>;
    };

    export template <EmulatorUIInterface Engine>
    class ui
    {
    public:
        explicit ui(Engine& engine)
            : engine { engine }
            , sdl_session { utils::value_or_panic(sdl::session::create(sdl::init_flags::video) )}
            , sdl_window { utils::value_or_panic(sdl::window::create("gameboy-cpp", 160, 144, sdl::window_flags::resizable)) }
            , sdl_renderer { utils::value_or_panic(sdl::renderer::create(sdl_window)) }
            , sdl_texture { utils::value_or_panic(sdl::texture::create(sdl_renderer, sdl::pixel_format::rgb_24, sdl::texture_access::streaming, 160, 144)) }
            , imgui_context { utils::value_or_panic(imgui::context::create()) }
            , imgui_backend { utils::value_or_panic(imgui::sdl3_renderer_backend::create(sdl_renderer)) }
            , imgui_platform { utils::value_or_panic(imgui::sdl3_platform::with_renderer(sdl_window, sdl_renderer)) }
        {
            imgui::check_version();
            imgui::set_dark_style();

            utils::panic_on_error(sdl_renderer.set_vsync(sdl::renderer_vsync::disabled));
        }

        // TODO: rethink interface
        [[nodiscard]] bool quit_app_requested() const { return should_quit; }

        void render()
        {
            PROFILER_SCOPE("Engine Thread");

            if (should_quit)
            {
                return;
            }

            // TODO: other events
            const auto event = sdl::poll_event();

            should_quit = event
                .transform(&sdl::event::parse)
                .transform([] (const auto& e) { return std::holds_alternative<sdl::quit_event>(e); })
                .value_or(false);

            if (event.has_value())
            {
                std::ignore = imgui_platform.process_event(event.value());
            }

            imgui_backend.new_frame();
            imgui_platform.new_frame();
            imgui::new_frame();

            if (const auto main_menu = imgui::main_menu_bar(); main_menu)
            {
                if (const auto file_menu = imgui::menu("File"); file_menu)
                {
                    if (imgui::menu_item("Open Rom"))
                    {
                        // TODO: open_file should be able to accept const span-like objects
                        std::array<pfd::filter, 1> rom_filter { { "Rom Files", "*.gb" } };

                        if (const auto rom_selection = pfd::open_file("Select Rom", std::nullopt, rom_filter); rom_selection.has_value())
                        {
                            // TODO: rethink this. Proper error handling
                            if (const auto rom_data = utils::read_binary_file(rom_selection.value()); rom_data.has_value())
                            {
                                // TODO: manual reset?
                                engine.load_rom(rom_data.value());
                            }
                            else
                            {
                                pfd::error_dialog("Error loading rom", rom_data.error());
                            }
                        }
                    }
                }
            }

            if (const auto viewport = imgui::window("Viewport"); viewport)
            {
                auto [lock, surface] = utils::value_or_panic(sdl::lock_to_surface(sdl_texture));
                std::ranges::copy(engine.framebuffer(), static_cast<std::uint8_t*>(surface.pixels()));

                const imgui::vec2 viewport_size = imgui::get_available_content_space();
                const imgui::texture_id texture_id = reinterpret_cast<imgui::texture_id>(sdl::internal::native::get_handle(sdl_texture));
                imgui::image(texture_id, viewport_size);
            }

            imgui::render();
            utils::panic_on_error(sdl_renderer.clear());
            imgui_backend.render();
            utils::panic_on_error(sdl_renderer.present());
        }

    private:
        Engine& engine;

        sdl::session sdl_session;
        sdl::window sdl_window;
        sdl::renderer sdl_renderer;
        sdl::texture sdl_texture;

        imgui::context imgui_context;
        imgui::sdl3_renderer_backend imgui_backend;
        imgui::sdl3_platform imgui_platform;

        bool should_quit { false };
    };

}