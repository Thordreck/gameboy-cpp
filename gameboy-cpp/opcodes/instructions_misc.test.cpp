
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

TEST_CASE("misc.nop does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::test_memory memory{};

	tests::execute_all_instruction_steps<opcodes::nop>(cpu);
	CHECK_EQ(cpu.pc, 0);
}
