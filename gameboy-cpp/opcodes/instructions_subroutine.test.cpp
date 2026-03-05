
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

namespace
{
	#define call_cc_n16_successful_jump_test_cases \
    tests::cc_test_case<opcodes::call_z_n16, tests::set_z_flag>, \
    tests::cc_test_case<opcodes::call_nz_n16, tests::unset_z_flag>, \
    tests::cc_test_case<opcodes::call_c_n16, tests::set_c_flag>, \
    tests::cc_test_case<opcodes::call_nc_n16, tests::unset_c_flag>

	#define call_cc_n16_unsuccessful_jump_test_cases \
    tests::cc_test_case<opcodes::call_z_n16, tests::unset_z_flag>, \
    tests::cc_test_case<opcodes::call_nz_n16, tests::set_z_flag>, \
    tests::cc_test_case<opcodes::call_c_n16, tests::unset_c_flag>, \
    tests::cc_test_case<opcodes::call_nc_n16, tests::set_c_flag>

	#define ret_cc_successful_jump_test_cases \
    tests::cc_test_case<opcodes::ret_z, tests::set_z_flag>, \
    tests::cc_test_case<opcodes::ret_nz, tests::unset_z_flag>, \
    tests::cc_test_case<opcodes::ret_c, tests::set_c_flag>, \
    tests::cc_test_case<opcodes::ret_nc, tests::unset_c_flag>

	#define ret_cc_unsuccessful_jump_test_cases \
    tests::cc_test_case<opcodes::ret_z, tests::unset_z_flag>, \
    tests::cc_test_case<opcodes::ret_nz, tests::set_z_flag>, \
    tests::cc_test_case<opcodes::ret_c, tests::unset_c_flag>, \
    tests::cc_test_case<opcodes::ret_nc, tests::set_c_flag>

	#define rst_vec_test_cases \
    tests::rst_vec_test_case<opcodes::rst_00, 0x00>, \
    tests::rst_vec_test_case<opcodes::rst_08, 0x08>, \
    tests::rst_vec_test_case<opcodes::rst_10, 0x10>, \
    tests::rst_vec_test_case<opcodes::rst_18, 0x18>, \
    tests::rst_vec_test_case<opcodes::rst_20, 0x20>, \
    tests::rst_vec_test_case<opcodes::rst_28, 0x28>, \
    tests::rst_vec_test_case<opcodes::rst_30, 0x30>, \
    tests::rst_vec_test_case<opcodes::rst_38, 0x38>
}

TEST_CASE("subroutine.call_n16 pushes pc into stack and jumps to direction")
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t pc_start = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.pc() = pc_start;
	cpu.sp() = stack_origin;
	cpu.memory().write(pc_start, 0xFF);
	cpu.memory().write(pc_start + 1, 0xAA);

	tests::execute_all_machine_cycles<opcodes::call_n16>(cpu);

	CHECK_EQ(cpu.pc(), 0XAAFF);
	CHECK_EQ(cpu.memory().read(stack_origin - 2), 0xCF);
	CHECK_EQ(cpu.memory().read(stack_origin - 1), 0xAB);
}

TEST_CASE_TEMPLATE("subroutine.call_cc_n16 executes jump when condition is met", test, call_cc_n16_successful_jump_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t pc_start = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.pc() = pc_start;
	cpu.sp() = stack_origin;
	cpu.memory().write(pc_start, 0xFF);
	cpu.memory().write(pc_start + 1, 0xAA);

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), 0XAAFF);
	CHECK_EQ(cpu.memory().read(stack_origin - 2), 0xCF);
	CHECK_EQ(cpu.memory().read(stack_origin - 1), 0xAB);
}

TEST_CASE_TEMPLATE("subroutine.call_cc_n16 does not execute jump when condition is met", test, call_cc_n16_unsuccessful_jump_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t pc_start = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.pc() = pc_start;
	cpu.sp() = stack_origin;
	cpu.memory().write(pc_start, 0xFF);
	cpu.memory().write(pc_start + 1, 0xAA);

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), pc_start + 2);
	CHECK_EQ(cpu.memory().read(stack_origin - 2), 0);
	CHECK_EQ(cpu.memory().read(stack_origin - 1), 0);
}

TEST_CASE("subroutine.ret updates pc with value from stack")
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t expected_pc = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = stack_origin - 2;
	cpu.memory().write(stack_origin - 1, 0xAB);
	cpu.memory().write(stack_origin - 2, 0xCD);

	tests::execute_all_machine_cycles<opcodes::ret>(cpu);
	CHECK_EQ(cpu.pc(), expected_pc);
}

TEST_CASE_TEMPLATE("subroutine.ret_cc updates pc if condition is satisfied", test, ret_cc_successful_jump_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t expected_pc = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = stack_origin - 2;
	cpu.memory().write(stack_origin - 1, 0xAB);
	cpu.memory().write(stack_origin - 2, 0xCD);

	test::set_condition(cpu);
	test::execute(cpu);
	CHECK_EQ(cpu.pc(), expected_pc);
}

TEST_CASE_TEMPLATE("subroutine.ret_cc does not update pc if condition is not satisfied", test, ret_cc_unsuccessful_jump_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t expected_pc = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = stack_origin - 2;
	cpu.memory().write(stack_origin - 1, 0xAB);
	cpu.memory().write(stack_origin - 2, 0xCD);

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), 0);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}

TEST_CASE_TEMPLATE("subroutine.rst_vec updates sp properly", test, rst_vec_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t pc_start = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.pc() = pc_start;
	cpu.sp() = stack_origin;

	test::execute(cpu);

	CHECK_EQ(cpu.memory().read(stack_origin - 2), 0xCD);
	CHECK_EQ(cpu.memory().read(stack_origin - 1), 0xAB);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}

TEST_CASE_TEMPLATE("subroutine.rst_vec updates pc properly", test, rst_vec_test_cases)
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = 0xFFFE;
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), test::vector);
}
