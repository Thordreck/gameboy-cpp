
#include "doctest.h"

import cpu;
import opcodes;

TEST_CASE("jump_n16 updates pc correctly")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;

	opcodes::jp_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 0XAAFF);
}

TEST_CASE("jump_z_n16 sets expected pc when condition is met")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;
	cpu.registers.z_flag() = true;

	opcodes::jp_z_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 0XAAFF);
}

TEST_CASE("jump_z_n16 increments pc by three when condition is not met")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;
	cpu.registers.z_flag() = false;

	opcodes::jp_z_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 3);
}

TEST_CASE("jump_nz_n16 sets expected pc when condition is met")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;
	cpu.registers.z_flag() = false;

	opcodes::jp_nz_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 0XAAFF);
}

TEST_CASE("jump_nz_n16 increments pc by three when condition is not met")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;
	cpu.registers.z_flag() = true;

	opcodes::jp_nz_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 3);
}

TEST_CASE("jump_c_n16 sets expected pc when condition is met")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;
	cpu.registers.c_flag() = true;

	opcodes::jp_c_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 0XAAFF);
}

TEST_CASE("jump_c_n16 increments pc by three when condition is not met")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;
	cpu.registers.c_flag() = false;

	opcodes::jp_c_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 3);
}

TEST_CASE("jump_nc_n16 sets expected pc when condition is met")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;
	cpu.registers.c_flag() = false;

	opcodes::jp_nc_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 0XAAFF);
}

TEST_CASE("jump_nc_n16 increments pc by three when condition is not met")
{
	cpu::cpu cpu{};
	cpu.memory[1] = 0xFF;
	cpu.memory[2] = 0xAA;
	cpu.registers.c_flag() = true;

	opcodes::jp_nc_n16::execute(cpu);
	CHECK_EQ(cpu.pc, 3);
}
