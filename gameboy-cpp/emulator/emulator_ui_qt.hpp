#pragma once

#include <span>
#include <string>
#include <memory>
#include <cstdint>
#include <expected>
#include <functional>

namespace emulator
{
    constexpr std::uint8_t ui_framebuffer_width = 160;
    constexpr std::uint8_t ui_framebuffer_height = 144;
    constexpr std::uint8_t ui_framebuffer_num_channels = 3;
    constexpr std::size_t ui_framebuffer_size = ui_framebuffer_height * ui_framebuffer_width * ui_framebuffer_num_channels;

    using ui_load_rom_result_t = std::expected<void, std::string>;
    using ui_framebuffer_view_t = std::span<const std::uint8_t, ui_framebuffer_size>;
    using ui_framebuffer_t = std::array<std::uint8_t, ui_framebuffer_size>;

    struct backend_functions
    {
        std::function<bool()> has_rom;
        std::function<bool()> is_running;

        std::function<ui_load_rom_result_t(std::string_view)> load_rom;
        std::function<void()> resume;
        std::function<void()> pause;
        std::function<void()> stop;
        std::function<void(std::uint32_t)> step;

        std::function<std::uint8_t(std::uint32_t)> read_memory;

        std::function<float()> volume;
        std::function<void(float)> set_volume;

        std::function<bool()> muted;
        std::function<void(bool)> set_muted;
    };

    class qt_graphical_interface
    {
    public:
        qt_graphical_interface(int argc, char** argv) noexcept;
        ~qt_graphical_interface();

        void start_rendering();
        void stop_rendering();
        void present_frame(ui_framebuffer_view_t frame);
        int render(const backend_functions& backend);

    private:
        struct pimpl;
        std::unique_ptr<pimpl> imp;
    };

}

