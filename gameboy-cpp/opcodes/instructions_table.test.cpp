
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

namespace
{
	using namespace cpu::literals;

	struct fake_instruction_0
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = 1;
		}
	};

	struct fake_instruction_1
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().b() = 1;
		}
	};

	using test_instruction_table_builder = opcodes::instruction_table_builder<
		opcodes::instruction_definition<0x0, fake_instruction_0>,
		opcodes::instruction_definition<0x1, fake_instruction_1>>;
}

TEST_CASE("table.instruction table maps existing opcodes correctly")
{
	const auto instruction_table = test_instruction_table_builder::build();

	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	instruction_table[0x0].execute(cpu);
	instruction_table[0x1].execute(cpu);

	CHECK_EQ(cpu.reg().a(), 1);
	CHECK_EQ(cpu.reg().b(), 1);
}
