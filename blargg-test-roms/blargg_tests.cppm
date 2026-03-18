module;
#include "doctest.h"

export module blargg;

export import std;

import cpu;
import timer;
import memory;
import opcodes;
import graphics;
import emulator;
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
			data[pixel_data_pos + 3] = color.a;
		}

	private:
		std::array<std::uint8_t, graphics::lcd_width * graphics::lcd_height * 4> data {};
	};

}

namespace blargg
{
	export void run_test(
		std::string_view rom_file_path,
		std::string_view expected_output,
		const size_t max_num_machine_cycles)
	{
		const std::filesystem::path rom_file{ rom_file_path };
		REQUIRE(std::filesystem::exists(rom_file));
		const std::vector<std::uint8_t> rom_data = read_rom(rom_file);

		cpu::cpu cpu{ };
		cpu.pc() = 0x100;
		cpu.sp() = 0xFFFE;

		timer::timer_system timers{};
		timers.divider() = 0xAB;

		// graphics
		memory_lcd lcd_imp {};
		graphics::lcd lcd(lcd_imp);
		graphics::pixel_processing_unit ppu(lcd);

		interrupts::interrupt_registers interrupts{};
		emulator::io_hram_interrupt_memory_page memory_page{ timers, interrupts, ppu };

		std::array<memory::memory_data_t, 0xFF00> memory{};
		std::ranges::copy(rom_data, memory.begin());
		auto memory_region = memory::map(memory);

		auto memory_map = memory::build_memory_map(memory_region, memory_page);
		auto memory_bus = memory::memory_bus{ memory_map };

		memory::connect(memory_bus, cpu, timers);

		std::string result{};
		const size_t max_num_timer_cycles = max_num_machine_cycles * 4;

		emulator::default_instructions_provider instructions{};
		emulator::default_interrupt_handler interrupts_handler{};
		emulator::cpu_runner runner{ cpu, instructions, interrupts_handler };

		for (size_t i = 0; i < max_num_timer_cycles; i++)
		{
			runner.tick();
			timers.tick();

			read_io_result_output(memory_bus, result);

			if (result == expected_output)
			{
				break;
			}
		}

		REQUIRE_EQ(expected_output, result);
		std::cout << result;
	}
}
