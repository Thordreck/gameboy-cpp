module;
#include "doctest.h"

export module blargg;

export import std;

import cpu;
import timer;
import memory;
import opcodes;
import emulator;
import interrupts;

namespace
{
	std::vector<std::uint8_t> read_rom(std::filesystem::path filepath)
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

		interrupts::interrupt_registers interrupts{};
		emulator::io_hram_interrupt_memory_page memory_page{ timers, interrupts };

		std::array<memory::memory_data_t, 0xFF00> memory{};
		std::copy(rom_data.cbegin(), rom_data.cend(), memory.begin());
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
