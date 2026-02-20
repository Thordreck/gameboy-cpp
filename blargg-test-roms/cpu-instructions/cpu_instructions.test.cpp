
#include "doctest.h"

import cpu;
import opcodes;
import std;

namespace
{
	using namespace opcodes;

	struct test_opcode_decoder
	{
		static instruction_fn_t decode(const cpu::cpu& cpu, const std::uint8_t opcode)
		{
			if (opcode != prefix_opcode)
			{
				REQUIRE_MESSAGE(
					default_instruction_table[opcode] != nullptr
					, std::format("Unknown opcode {:x} at pc {:x}", opcode, cpu.pc().value()));

				return default_opcode_decoder::decode(cpu, opcode);
			}

			const std::uint8_t prefixed_opcode = cpu.memory()[cpu.pc() + 1];

			REQUIRE_MESSAGE(
				default_prefixed_instruction_table[prefixed_opcode] != nullptr, 
				std::format("Unknown prefixed opcode {:x} pc {:x}", prefixed_opcode, cpu.pc().value()));

			return default_opcode_decoder::decode(cpu, opcode);
		}
	};

	using test_instruction_pipeline = instructions_pipeline<
		default_opcode_fetcher,
		test_opcode_decoder,
		default_instruction_executor>;

	std::vector<std::uint8_t> read_rom(std::filesystem::path filepath)
	{
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	}

	void read_io_result_output(cpu::cpu& cpu, std::string& result)
	{
		constexpr cpu::memory_bus::index_t sb = 0xFF01;
		constexpr cpu::memory_bus::index_t sc = 0xFF02;

		constexpr cpu::memory_bus::type_t transfer_start = 0x81;

		if (cpu.memory()[sc] == transfer_start)
		{
			result += cpu.memory()[sb];
			cpu.memory()[sc] = 0;
		}
	}

	void run_test(
		std::string_view rom_file_path,
		std::string_view expected_output,
		const size_t max_num_instructions)
	{
		const std::filesystem::path rom_file{ rom_file_path };
		REQUIRE(std::filesystem::exists(rom_file));

		const std::vector<std::uint8_t> rom_data = read_rom(rom_file);

		std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
		std::copy(rom_data.cbegin(), rom_data.cend(), memory.begin());

		cpu::cpu cpu{ memory };
		cpu.pc() = 0x100;

		std::string result{};

		for (size_t i = 0; i < max_num_instructions; i++)
		{
			test_instruction_pipeline::step(cpu);
			read_io_result_output(cpu, result);

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
	run_test("03-op sp,hl.gb", "03-op sp,hl\n\n\nPassed\n", 16e5);
}

TEST_CASE("blargg.cpu_instrs.04-op r,imm")
{
	run_test("04-op r,imm.gb", "04-op r,imm\n\n\nPassed\n", 16e5);
}

TEST_CASE("blargg.cpu_instrs.05-op rp")
{
	run_test("05-op rp.gb", "05-op rp\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.06-ld r,r")
{
	run_test("06-ld r,r.gb", "06-ld r,r\n\n\nPassed\n", 30e4);
}

TEST_CASE("blargg.cpu_instrs.07-jr,jp,call,ret,rst")
{
	run_test("07-jr,jp,call,ret,rst.gb", "07-jr,jp,call,ret,rst\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.08-misc instrs")
{
	run_test("08-misc instrs.gb", "08-misc instrs\n\n\nPassed\n", 30e4);
}

TEST_CASE("blargg.cpu_instrs.09-op r,r")
{
	run_test("09-op r,r.gb", "09-op r,r\n\n\nPassed\n", 10e6);
}

TEST_CASE("blargg.cpu_instrs.10-bit ops")
{
	run_test("10-bit ops.gb", "10-bit ops\n\n\nPassed\n",  30e6);
}

TEST_CASE("blargg.cpu_instrs.11-op a,(hl)")
{
	run_test("11-op a,(hl).gb", "11-op a,(hl)\n\n\nPassed\n", 10e6);
}
