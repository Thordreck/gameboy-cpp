
#include "doctest.h"

import cpu;
import std;
import opcodes;

namespace
{
	struct fake_instruction_0
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = 1;
		}
	};

	struct fake_instruction_1
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().b() = 1;
		}
	};

	using test_instruction_table_builder = opcodes::instruction_table_builder<
		opcodes::instruction_definition<0x0, fake_instruction_0>,
		opcodes::instruction_definition<0x1, fake_instruction_1>>;
}

TEST_CASE("instruction table maps existing opcodes correctly")
{
	const auto instruction_table = test_instruction_table_builder::build();

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	instruction_table[0x0](cpu);
	instruction_table[0x1](cpu);

	CHECK_EQ(cpu.reg().a(), 1);
	CHECK_EQ(cpu.reg().b(), 1);
}
