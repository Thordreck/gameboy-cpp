
#include "doctest.h"

import cpu;
import std;
import opcodes;

TEST_CASE("nop increments program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::nop::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}
