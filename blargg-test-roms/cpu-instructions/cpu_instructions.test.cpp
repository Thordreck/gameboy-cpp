
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

	// TODO: implemnt this properly
	for (int i = 0; i < 200000; i++)
	{
		const std::uint8_t opcode = cpu.memory()[cpu.pc()];

		std::cout << std::format("Opcode {:x}. PC {:x}\n", opcode, static_cast<cpu::program_counter::type_t>(cpu.pc()));

		REQUIRE_MESSAGE(opcode < instruction_table.size(), std::format("Unknown opcode: {:x}", opcode));
		REQUIRE_MESSAGE(instruction_table[opcode] != nullptr, std::format("Unknown opcode {:x}", opcode));

		instruction_table[opcode](cpu);
	}
}
