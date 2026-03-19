#include "doctest.h"

import std;
import cpu;
import timer;
import memory;
import graphics;
import emulator;
import stb_image;

namespace
{
    std::vector<std::uint8_t> read_rom(const std::filesystem::path& filepath)
    {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        return { std::istreambuf_iterator(file), std::istreambuf_iterator<char>() };
    }

    class memory_lcd
    {
    public:
        void set_pixel(const graphics::coords_2d coords, const graphics::color color)
        {
            const std::size_t pixel_data_pos = (coords.y * graphics::lcd_width + coords.x) * 3;
            CHECK(pixel_data_pos + 2 < data.size());

            data[pixel_data_pos] = color.r;
            data[pixel_data_pos + 1] = color.g;
            data[pixel_data_pos + 2] = color.b;
            //data[pixel_data_pos + 3] = color.a;
        }

        [[nodiscard]] auto& get_data() const { return data; }

    private:
        std::array<std::uint8_t, graphics::lcd_width * graphics::lcd_height * 3> data {};
    };

}

TEST_CASE("acid.PPU generates output equals to reference image")
{
    // rom
    const std::filesystem::path rom_file{ "dmg-acid2.gb" };
    REQUIRE(std::filesystem::exists(rom_file));
    const std::vector<std::uint8_t> rom_data = read_rom(rom_file);

    // cpu
    cpu::cpu cpu{ };
    cpu.pc() = 0x100;
    cpu.sp() = 0xFFFE;

    // timer
    timer::timer_system timers{};
    timers.divider() = 0xAB;

    // interrupts
    interrupts::interrupt_registers interrupts{};

    // graphics
    std::array<memory::memory_data_t, graphics::vram_size> vram {};

    memory_lcd lcd_imp {};
    graphics::lcd lcd(lcd_imp);

    graphics::pixel_processing_unit ppu(lcd, vram);

    // memory
    emulator::vram_memory_page vram_page { ppu, std::span {vram } };
    emulator::io_hram_interrupt_memory_page io_hram_interrupt_page{ timers, interrupts, ppu };

    std::array<memory::memory_data_t, vram_page.start> fallback_memory_1{};
    std::array<memory::memory_data_t, 0x5F00> fallback_memory_2{};

    std::ranges::copy(rom_data, fallback_memory_1.begin());
    auto fallback_memory_page_1 = memory::map(fallback_memory_1);
    auto fallback_memory_page_2 = memory::map<0x5F00, 0xA000, 0xFEFF>(fallback_memory_2);

    auto memory_map = memory::build_memory_map(
        fallback_memory_page_1,
        vram_page,
        fallback_memory_page_2,
        io_hram_interrupt_page);

    auto memory_bus = memory::memory_bus{ memory_map };
    memory::connect(memory_bus, cpu, timers, ppu);

    // cpu runner
    emulator::default_instructions_provider instructions{};
    emulator::default_interrupt_handler interrupts_handler{};
    emulator::cpu_runner runner{ cpu, instructions, interrupts_handler };

    // The rom draws a single frame.
    while (ppu.scanline() < 153)
    {
        runner.tick();
        timers.tick();
        ppu.tick();
    }

    REQUIRE_MESSAGE(
        std::ranges::all_of(lcd_imp.get_data(), [] (const auto color) { return color != 0; }),
        "Not all pixels have been drawn into lcd");

    const auto output_filepath = std::filesystem::temp_directory_path() / "test.png";
    constexpr stb::image_metadata output_metadata
    {
        graphics::lcd_width,
        graphics::lcd_height,
        3
    };

    // TODO: generate resulting png data and compare it with reference image
    const auto result = stb::write_png(output_filepath, lcd_imp.get_data().data(), output_metadata);
    REQUIRE_MESSAGE(result.has_value(), std::format("Result vram png failed. {}", result.error()));

    std::cout << "Image generated at " << output_filepath << "\n";
}
