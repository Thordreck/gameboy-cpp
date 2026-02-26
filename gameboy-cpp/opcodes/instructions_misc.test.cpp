
#include "doctest.h"

import cpu;
import std;
import opcodes;
import tests;

TEST_CASE("misc.nop does not increment program counter")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	tests::execute_all_machine_cycles<opcodes::nop>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}
