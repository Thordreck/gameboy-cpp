
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

namespace
{
	#define jp_cc_n16_successful_jump_test_cases \
    tests::cc_test_case<opcodes::jp_z_n16, tests::set_z_flag>, \
    tests::cc_test_case<opcodes::jp_nz_n16, tests::unset_z_flag>, \
    tests::cc_test_case<opcodes::jp_c_n16, tests::set_c_flag>, \
    tests::cc_test_case<opcodes::jp_nc_n16, tests::unset_c_flag>

	#define jp_cc_n16_unsuccessful_jump_test_cases \
    tests::cc_test_case<opcodes::jp_z_n16, tests::unset_z_flag>, \
    tests::cc_test_case<opcodes::jp_nz_n16, tests::set_z_flag>, \
    tests::cc_test_case<opcodes::jp_c_n16, tests::unset_c_flag>, \
    tests::cc_test_case<opcodes::jp_nc_n16, tests::set_c_flag>

	#define jr_cc_n16_successful_jump_test_cases \
    tests::cc_test_case<opcodes::jr_z_n16, tests::set_z_flag>, \
    tests::cc_test_case<opcodes::jr_nz_n16, tests::unset_z_flag>, \
    tests::cc_test_case<opcodes::jr_c_n16, tests::set_c_flag>, \
    tests::cc_test_case<opcodes::jr_nc_n16, tests::unset_c_flag>

	#define jr_cc_n16_unsuccessful_jump_test_cases \
    tests::cc_test_case<opcodes::jr_z_n16, tests::unset_z_flag>, \
    tests::cc_test_case<opcodes::jr_nz_n16, tests::set_z_flag>, \
    tests::cc_test_case<opcodes::jr_c_n16, tests::unset_c_flag>, \
    tests::cc_test_case<opcodes::jr_nc_n16, tests::set_c_flag>
}

TEST_CASE("jump.jp_n16 updates pc correctly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.memory().write(0, 0xFF);
	cpu.memory().write(1, 0xAA);

	tests::execute_all_machine_cycles<opcodes::jp_n16>(cpu);
	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE_TEMPLATE("jump.jp_cc_n16 sets expected pc when condition is met", test, jp_cc_n16_successful_jump_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.memory().write(0, 0xFF);
	cpu.memory().write(1, 0xAA);

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE_TEMPLATE("jump.jp_cc_n16 sets expected pc when condition is not met", test, jp_cc_n16_unsuccessful_jump_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.memory().write(0, 0xFF);
	cpu.memory().write(1, 0xAA);

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("jump.jr_n16 updates pc correctly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.memory().write(0, 2);

	tests::execute_all_machine_cycles<opcodes::jr_n16>(cpu);
	CHECK_EQ(cpu.pc(), 3);

	cpu.memory().write(3, -2);

	tests::execute_all_machine_cycles<opcodes::jr_n16>(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("jump.jr_cc_n16 sets expected pc when condition is met", test, jr_cc_n16_successful_jump_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.memory().write(0, 2);

	test::set_condition(cpu);
	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 3);

	cpu.memory().write(3, -2);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("jump.jr_cc_n16 sets expected pc when condition is not met", test, jr_cc_n16_unsuccessful_jump_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.memory().write(0, 2);

	test::set_condition(cpu);
	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("jump.jp_hl sets expected pc")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::jp_hl>(cpu);
	CHECK_EQ(cpu.pc(), 0);

	cpu.reg().hl() = 0xABCD;
	tests::execute_all_machine_cycles<opcodes::jp_hl>(cpu);
	CHECK_EQ(cpu.pc(), 0xABCD);
}
