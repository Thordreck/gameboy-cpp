
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

namespace
{
	#define pop_r16_test_cases \
    tests::r16_test_case<opcodes::pop_bc, tests::get_bc>, \
    tests::r16_test_case<opcodes::pop_de, tests::get_de>, \
    tests::r16_test_case<opcodes::pop_hl, tests::get_hl>

	#define push_r16_test_cases \
    tests::r16_test_case<opcodes::push_bc, tests::get_bc>, \
    tests::r16_test_case<opcodes::push_de, tests::get_de>, \
    tests::r16_test_case<opcodes::push_hl, tests::get_hl>
}

TEST_CASE_TEMPLATE("stack.pop_r16 updates reg with correct value from stack", test, pop_r16_test_cases)
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = stack_origin - 2;
	cpu.memory().write(stack_origin - 2, 0xCD);
	cpu.memory().write(stack_origin - 1, 0xAB);

	test::execute(cpu);

	CHECK_EQ(static_cast<std::uint16_t>(test::reg(cpu)), test_value);
	CHECK_EQ(cpu.pc(), 0);
	CHECK_EQ(cpu.sp(), stack_origin);
}


TEST_CASE_TEMPLATE("stack.push_r16 updates stack with correct value from register", test, push_r16_test_cases)
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };
	cpu.sp() = stack_origin;

	test::reg(cpu) = test_value;
	test::execute(cpu);

	CHECK_EQ(cpu.memory().read(stack_origin - 2), 0xCD);
	CHECK_EQ(cpu.memory().read(stack_origin - 1), 0xAB);
	CHECK_EQ(cpu.pc(), 0);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}


TEST_CASE("stack.ld_sp_n16 updates sp with correct value from memory")
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	memory.bus().write(0, 0xCD);
	memory.bus().write(1, 0xAB);

	tests::execute_all_machine_cycles<opcodes::ld_sp_n16>(cpu);

	CHECK_EQ(cpu.sp(), test_value);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("stack.add_sp_e8 updates stack pointer properly")
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = 0;
	memory.bus().write(0, 127);

	tests::execute_all_machine_cycles<opcodes::add_sp_e8>(cpu);
	CHECK_EQ(cpu.sp(), 127);

	cpu.pc() = 0;
	memory.bus().write(0, -127);

	tests::execute_all_machine_cycles<opcodes::add_sp_e8>(cpu);
	CHECK_EQ(cpu.sp(), 0x0);
}

TEST_CASE("stack.add_sp_e8 updates flags properly")
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = 0;
	memory.bus().write(0, 0xF);
	tests::execute_all_machine_cycles<opcodes::add_sp_e8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	memory.bus().write(0, 0xF);
	tests::execute_all_machine_cycles<opcodes::add_sp_e8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	memory.bus().write(0, 0xFF);
	tests::execute_all_machine_cycles<opcodes::add_sp_e8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("stack.add_sp_e8 updates program counter properly")
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	tests::execute_all_machine_cycles<opcodes::add_sp_e8>(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("stack.ld_hl_sp_e8 updates hl properly")
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = 127;
	memory.bus().write(0, 0);

	tests::execute_all_machine_cycles<opcodes::ld_hl_sp_e8>(cpu);
	CHECK_EQ(cpu.reg().hl(), 127);

	cpu.pc() = 0;
	cpu.sp() = 127;
	memory.bus().write(0, -127);

	tests::execute_all_machine_cycles<opcodes::ld_hl_sp_e8>(cpu);
	CHECK_EQ(cpu.reg().hl(), 0x0);
}

TEST_CASE("stack.ld_hl_sp_e8 updates flags properly")
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = 0;
	memory.bus().write(0, 0xF);
	tests::execute_all_machine_cycles<opcodes::ld_hl_sp_e8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	cpu.sp() = 0xF;
	memory.bus().write(0, 0xF);
	tests::execute_all_machine_cycles<opcodes::ld_hl_sp_e8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	cpu.sp() = 0xFFFF;
	memory.bus().write(0, 0xFF);
	tests::execute_all_machine_cycles<opcodes::ld_hl_sp_e8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("stack.ld_hl_sp_e8 updates program counter properly")
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	tests::execute_all_machine_cycles<opcodes::ld_hl_sp_e8>(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("stack.pop_af updates reg with correct value from stack")
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = stack_origin - 2;
	cpu.memory().write(stack_origin - 2, 0xCD);
	cpu.memory().write(stack_origin - 1, 0xAB);

	tests::execute_all_machine_cycles<opcodes::pop_af>(cpu);

	CHECK_EQ(cpu.reg().af(), test_value & 0xFFF0);
	CHECK_EQ(cpu.pc(), 0);
	CHECK_EQ(cpu.sp(), stack_origin);
}

TEST_CASE("stack.push_af updates stack with correct value from register")
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	cpu.sp() = stack_origin;
	cpu.reg().af() = test_value;

	tests::execute_all_machine_cycles<opcodes::push_af>(cpu);

	CHECK_EQ(cpu.memory().read(stack_origin - 2), 0xC0);
	CHECK_EQ(cpu.memory().read(stack_origin - 1), 0xAB);
	CHECK_EQ(cpu.pc(), 0);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}
