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

    graphics::oam_dma oam_dma {};
    graphics::pixel_processing_unit ppu(lcd);

    // memory
    emulator::vram_memory_page vram_page { vram };
    emulator::io_hram_interrupt_memory_page io_hram_interrupt_page{ timers, interrupts, ppu };
    emulator::oam_memory_page oam_memory_page { oam_dma };

    std::array<memory::memory_data_t, 0x4000> rom_bank_0{};
    std::array<memory::memory_data_t, 0x4000> rom_bank_n{};
    std::array<memory::memory_data_t, 0x2000> external_ram{};
    std::array<memory::memory_data_t, 0x1000> work_ram_0{};
    std::array<memory::memory_data_t, 0x1000> work_ram_1{};
    std::array<memory::memory_data_t, 0x1E00> echo_ram{};

    std::ranges::copy(rom_data | std::views::take(rom_bank_0.size()), rom_bank_0.begin());
    std::ranges::copy(rom_data | std::views::drop(rom_bank_0.size()), rom_bank_n.begin());

    auto rom_bank_0_page = memory::map(rom_bank_0);
    auto rom_bank_n_page = memory::map<0x4000, 0x4000, 0x7FFF>(rom_bank_n);
    auto external_ram_page = memory::map<0x2000, 0xA000, 0xBFFF>(external_ram);
    auto work_ram_0_page = memory::map<0x1000, 0xC000, 0xCFFF>(work_ram_0);
    auto work_ram_1_page = memory::map<0x1000, 0xD000, 0xDFFF>(work_ram_1);
    auto echo_ram_page = memory::map<0x1E00, 0xE000, 0xFDFF>(echo_ram);

    auto memory_map = memory::build_memory_map(
        rom_bank_0_page,
        rom_bank_n_page,
        vram_page,
        external_ram_page,
        work_ram_0_page,
        work_ram_1_page,
        echo_ram_page,
        oam_memory_page,
        io_hram_interrupt_page);

    graphics::vram_access_policy vram_policy { ppu };
    graphics::oam_dma_access_policy oam_dma_policy { oam_dma };
    graphics::oam_ppu_access_policy oam_ppu_policy { ppu, oam_dma };

    memory::memory_bus raw_memory_bus { memory_map };
    memory::memory_bus cpu_memory_bus { memory_map, vram_policy, oam_dma_policy, oam_ppu_policy };
    memory::memory_bus ppu_memory_bus { memory_map, oam_ppu_policy };

    memory::connect(cpu_memory_bus, cpu, timers);
    memory::connect(ppu_memory_bus, ppu);
    memory::connect(raw_memory_bus, oam_dma);

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
        oam_dma.tick();
    }

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
