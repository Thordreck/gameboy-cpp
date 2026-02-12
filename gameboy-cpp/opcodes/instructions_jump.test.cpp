
#include "doctest.h"

import cpu;
import opcodes;

TEST_CASE("jump_n16 updates pc correctly")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;

	opcodes::jp_n16::execute(cpu);
	CHECK_EQ(cpu.pc.as_bytes(), 0XAAFF);
}
