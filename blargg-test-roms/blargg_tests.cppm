module;
#include "doctest.h"

export module blargg;

export import std;

import cpu;
import timer;
import memory;
import opcodes;
import graphics;
import emulator.core;
import interrupts;

namespace
{
	std::vector<std::uint8_t> read_rom(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	}

	void read_io_result_output(
		memory::memory_bus& memory,
		std::string& result)
	{
		constexpr memory::memory_address_t sb = 0xFF01;
		constexpr memory::memory_address_t sc = 0xFF02;
		constexpr memory::memory_data_t transfer_start = 0x81;

		if (memory.read(sc) == transfer_start)
		{
			result += memory.read(sb);
			memory.write(sc, 0);
		}
	}

	class memory_lcd
	{
	public:
		void set_pixel(const graphics::coords_2d coords, const graphics::color color)
		{
			const std::uint8_t pixel_data_pos = (coords.x * graphics::lcd_width + coords.y) * 4;
			CHECK(pixel_data_pos < data.size());

			data[pixel_data_pos] = color.r;
			data[pixel_data_pos + 1] = color.g;
			data[pixel_data_pos + 2] = color.b;
		}

	private:
		std::array<std::uint8_t, graphics::lcd_width * graphics::lcd_height * graphics::num_color_channels> data {};
	};

}

namespace blargg
{
	export void run_test(
		std::string_view rom_file_path,
		std::string_view expected_output,
		const size_t max_num_machine_cycles)
	{
		// rom
		const std::filesystem::path rom_file{ rom_file_path };
		REQUIRE(std::filesystem::exists(rom_file));
		const std::vector<std::uint8_t> rom_data = read_rom(rom_file);

		// cpu
		cpu::cpu cpu{ };
		cpu.pc() = 0x100;
		cpu.sp() = 0xFFFE;

		// rimer
		timer::timer_system timers{};
		timers.divider() = 0xAB;

		// interrupts
		interrupts::interrupt_registers interrupts{};

		// graphics
		std::array<memory::memory_data_t, graphics::vram_size> vram {};
		std::array<memory::memory_data_t, 0x7F> hram {};

		memory_lcd lcd_imp {};
		graphics::lcd lcd(lcd_imp);
		graphics::pixel_processing_unit ppu(lcd);

		// Memory
		emulator::vram_memory_page_t vram_page { vram };
		emulator::io_hram_interrupt_memory_page io_hram_interrupt_page{ timers, interrupts, ppu, hram };

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

		graphics::vram_access_policy vram_policy { ppu };

		memory::memory_bus cpu_memory_bus { memory_map, vram_policy };
		memory::memory_bus ppu_memory_bus { memory_map };

		memory::connect(cpu_memory_bus, cpu, timers);
		memory::connect(ppu_memory_bus, cpu, timers, ppu);

		// cpu runner
		emulator::default_cpu_runner runner{ cpu };

		// results
		std::string result{};
		const size_t max_num_timer_cycles = max_num_machine_cycles * 4;

		for (size_t i = 0; i < max_num_timer_cycles; i++)
		{
			runner.tick();
			timers.tick();
			ppu.tick();

			read_io_result_output(ppu_memory_bus, result);

			if (result == expected_output)
			{
				break;
			}
		}

		REQUIRE_EQ(expected_output, result);
		std::cout << result;
	}
}
