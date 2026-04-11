
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

TEST_CASE("misc.nop does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::nop>(cpu);
	CHECK_EQ(cpu.pc, 0);
}
