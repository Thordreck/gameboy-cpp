
#include "doctest.h"

import opcodes;
import std;
import cpu;

TEST_CASE("scf set flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().n_flag() = true;
	cpu.reg().h_flag() = true;
	cpu.reg().c_flag() = false;

	opcodes::scf::execute(cpu);

	CHECK_FALSE(cpu.reg().n_flag());
	CHECK_FALSE(cpu.reg().h_flag());
	CHECK(cpu.reg().c_flag());
}

TEST_CASE("scf increments program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::scf::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("ccf set flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().n_flag() = true;
	cpu.reg().h_flag() = true;
	cpu.reg().c_flag() = false;

	opcodes::ccf::execute(cpu);

	CHECK_FALSE(cpu.reg().n_flag());
	CHECK_FALSE(cpu.reg().h_flag());
	CHECK(cpu.reg().c_flag());

	opcodes::ccf::execute(cpu);

	CHECK_FALSE(cpu.reg().n_flag());
	CHECK_FALSE(cpu.reg().h_flag());
	CHECK_FALSE(cpu.reg().c_flag());
}

TEST_CASE("ccf increments program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::ccf::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}