
#include "doctest.h"

import std;
import cpu;
import tests;
import opcodes;

TEST_CASE("carry.scf set flags properly")
{
	cpu::cpu_state cpu{ };
	tests::test_memory memory{};

	cpu.reg.n_flag() = true;
	cpu.reg.h_flag() = true;
	cpu.reg.c_flag() = false;

	tests::execute_all_instruction_steps<opcodes::scf>(cpu);

	CHECK_FALSE(cpu.reg.n_flag());
	CHECK_FALSE(cpu.reg.h_flag());
	CHECK(cpu.reg.c_flag());
}

TEST_CASE("carry.scf does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::execute_all_instruction_steps<opcodes::scf>(cpu);

	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("carry.ccf set flags properly")
{
	cpu::cpu_state cpu{ };
	tests::test_memory memory{};

	cpu.reg.n_flag() = true;
	cpu.reg.h_flag() = true;
	cpu.reg.c_flag() = false;

	tests::execute_all_instruction_steps<opcodes::ccf>(cpu);

	CHECK_FALSE(cpu.reg.n_flag());
	CHECK_FALSE(cpu.reg.h_flag());
	CHECK(cpu.reg.c_flag());

	tests::execute_all_instruction_steps<opcodes::ccf>(cpu);

	CHECK_FALSE(cpu.reg.n_flag());
	CHECK_FALSE(cpu.reg.h_flag());
	CHECK_FALSE(cpu.reg.c_flag());
}

TEST_CASE("carry.ccf does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::test_memory memory{};

	tests::execute_all_instruction_steps<opcodes::ccf>(cpu);
	CHECK_EQ(cpu.pc, 0);
}