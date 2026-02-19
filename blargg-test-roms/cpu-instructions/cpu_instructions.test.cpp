
#include "doctest.h"

import cpu;
import opcodes;
import std;

namespace
{
	std::vector<std::uint8_t> read_rom(std::filesystem::path filepath)
	{
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	}

	opcodes::instruction_fn_t fetch_instruction(
		const cpu::cpu& cpu, 
		const opcodes::instruction_table& instructions,
		const opcodes::instruction_table& prefixed_instructions)
	{ 
		const std::uint8_t opcode = cpu.memory()[cpu.pc()];

		if (opcode != opcodes::prefix_opcode)
		{
			REQUIRE_MESSAGE(instructions[opcode] != nullptr, std::format("Unknown opcode {:x}", opcode));
			return instructions[opcode];
		}

		const std::uint8_t prefixed_opcode = cpu.memory()[cpu.pc() + 1];

		REQUIRE_MESSAGE(prefixed_instructions[prefixed_opcode] != nullptr, std::format("Unknown prefixed opcode {:x}", prefixed_opcode));
		return prefixed_instructions[prefixed_opcode];
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
}

TEST_CASE("blargg.06-ld r,r")
{
	const std::filesystem::path rom_file{ "06-ld r,r.gb" };
	REQUIRE(std::filesystem::exists(rom_file));

	const std::vector<std::uint8_t> rom_data = read_rom(rom_file);

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	std::copy(rom_data.cbegin(), rom_data.cend(), memory.begin());

	cpu::cpu cpu{ memory };
	cpu.pc() = 0x100;

	constexpr auto instruction_table = opcodes::default_instruction_table_builder::build();
	constexpr auto prefix_instruction_table = opcodes::default_prefixed_instruction_table_builder::build();
	constexpr int expected_rom_instructions = 287000;
	
	std::string result{};

	for (int i = 0; i < expected_rom_instructions; i++)
	{
		const auto instruction = fetch_instruction(cpu, instruction_table, prefix_instruction_table);;
		instruction(cpu);
		read_io_result_output(cpu, result);
	}

	constexpr std::string_view expected_output = "06-ld r,r\n\n\nPassed\n";
	REQUIRE_EQ(expected_output, result);

	std::cout << result;
}
