
#include "doctest.h"

import cpu;
import opcodes;
import std;

TEST_CASE("jump_n16 updates pc correctly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;

	opcodes::jp_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE("jump_z_n16 sets expected pc when condition is met")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;
	cpu.reg().z_flag() = true;

	opcodes::jp_z_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE("jump_z_n16 increments pc by three when condition is not met")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;
	cpu.reg().z_flag() = false;

	opcodes::jp_z_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE("jump_nz_n16 sets expected pc when condition is met")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;
	cpu.reg().z_flag() = false;

	opcodes::jp_nz_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE("jump_nz_n16 increments pc by three when condition is not met")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;
	cpu.reg().z_flag() = true;

	opcodes::jp_nz_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE("jump_c_n16 sets expected pc when condition is met")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;
	cpu.reg().c_flag() = true;

	opcodes::jp_c_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE("jump_c_n16 increments pc by three when condition is not met")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;
	cpu.reg().c_flag() = false;

	opcodes::jp_c_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE("jump_nc_n16 sets expected pc when condition is met")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;
	cpu.reg().c_flag() = false;

	opcodes::jp_nc_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE("jump_nc_n16 increments pc by three when condition is not met")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;
	cpu.reg().c_flag() = true;

	opcodes::jp_nc_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 3);
}
