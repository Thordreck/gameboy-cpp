
#include "doctest.h"

import std;
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

	void run_test(
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
		emulator::io_hram_interrupt_memory_page memory_page { timers, interrupts };

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

TEST_CASE("blargg.cpu_instrs.01-special")
{
	run_test("01-special.gb", "01-special\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.02-interrupts")
{
	run_test("02-interrupts.gb", "02-interrupts\n\n\nPassed\n", 20e6);
}

TEST_CASE("blargg.cpu_instrs.03-op sp,hl")
{
	run_test("03-op sp,hl.gb", "03-op sp,hl\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.04-op r,imm")
{
	run_test("04-op r,imm.gb", "04-op r,imm\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.05-op rp")
{
	run_test("05-op rp.gb", "05-op rp\n\n\nPassed\n", 60e5);
}

TEST_CASE("blargg.cpu_instrs.06-ld r,r")
{
	run_test("06-ld r,r.gb", "06-ld r,r\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.07-jr,jp,call,ret,rst")
{
	run_test("07-jr,jp,call,ret,rst.gb", "07-jr,jp,call,ret,rst\n\n\nPassed\n", 60e5);
}

TEST_CASE("blargg.cpu_instrs.08-misc instrs")
{
	run_test("08-misc instrs.gb", "08-misc instrs\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.09-op r,r")
{
	run_test("09-op r,r.gb", "09-op r,r\n\n\nPassed\n", 30e6);
}

TEST_CASE("blargg.cpu_instrs.10-bit ops")
{
	run_test("10-bit ops.gb", "10-bit ops\n\n\nPassed\n",  30e6);
}

TEST_CASE("blargg.cpu_instrs.11-op a,(hl)")
{
	run_test("11-op a,(hl).gb", "11-op a,(hl)\n\n\nPassed\n", 30e6);
}
