
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

namespace
{
#define add_a_r8_test_cases \
    tests::r8_test_case<opcodes::add_a_b, tests::get_b>, \
    tests::r8_test_case<opcodes::add_a_c, tests::get_c>, \
    tests::r8_test_case<opcodes::add_a_d, tests::get_d>, \
    tests::r8_test_case<opcodes::add_a_e, tests::get_e>, \
    tests::r8_test_case<opcodes::add_a_h, tests::get_h>, \
    tests::r8_test_case<opcodes::add_a_l, tests::get_l>

#define inc_r8_test_cases \
    tests::r8_test_case<opcodes::inc_a, tests::get_a>, \
    tests::r8_test_case<opcodes::inc_b, tests::get_b>, \
    tests::r8_test_case<opcodes::inc_c, tests::get_c>, \
    tests::r8_test_case<opcodes::inc_d, tests::get_d>, \
    tests::r8_test_case<opcodes::inc_e, tests::get_e>, \
    tests::r8_test_case<opcodes::inc_h, tests::get_h>, \
    tests::r8_test_case<opcodes::inc_l, tests::get_l>

#define dec_r8_test_cases \
    tests::r8_test_case<opcodes::dec_a, tests::get_a>, \
    tests::r8_test_case<opcodes::dec_b, tests::get_b>, \
    tests::r8_test_case<opcodes::dec_c, tests::get_c>, \
    tests::r8_test_case<opcodes::dec_d, tests::get_d>, \
    tests::r8_test_case<opcodes::dec_e, tests::get_e>, \
    tests::r8_test_case<opcodes::dec_h, tests::get_h>, \
    tests::r8_test_case<opcodes::dec_l, tests::get_l>

#define inc_r16_test_cases \
    tests::r16_test_case<opcodes::inc_bc, tests::get_bc>, \
    tests::r16_test_case<opcodes::inc_de, tests::get_de>, \
    tests::r16_test_case<opcodes::inc_hl, tests::get_hl>

#define dec_r16_test_cases \
    tests::r16_test_case<opcodes::dec_bc, tests::get_bc>, \
    tests::r16_test_case<opcodes::dec_de, tests::get_de>, \
    tests::r16_test_case<opcodes::dec_hl, tests::get_hl>

#define cp_a_r8_test_cases \
    tests::r8_test_case<opcodes::cp_a_b, tests::get_b>, \
    tests::r8_test_case<opcodes::cp_a_c, tests::get_c>, \
    tests::r8_test_case<opcodes::cp_a_d, tests::get_d>, \
    tests::r8_test_case<opcodes::cp_a_e, tests::get_e>, \
    tests::r8_test_case<opcodes::cp_a_h, tests::get_h>, \
    tests::r8_test_case<opcodes::cp_a_l, tests::get_l>

#define adc_a_r8_test_cases \
    tests::r8_test_case<opcodes::adc_a_b, tests::get_b>, \
    tests::r8_test_case<opcodes::adc_a_c, tests::get_c>, \
    tests::r8_test_case<opcodes::adc_a_d, tests::get_d>, \
    tests::r8_test_case<opcodes::adc_a_e, tests::get_e>, \
    tests::r8_test_case<opcodes::adc_a_h, tests::get_h>, \
    tests::r8_test_case<opcodes::adc_a_l, tests::get_l>

#define add_hl_r16_test_cases \
    tests::r16_test_case<opcodes::add_hl_bc, tests::get_bc>, \
    tests::r16_test_case<opcodes::add_hl_de, tests::get_de>

#define sbc_a_r8_test_cases \
    tests::r8_test_case<opcodes::sbc_a_b, tests::get_b>, \
    tests::r8_test_case<opcodes::sbc_a_c, tests::get_c>, \
    tests::r8_test_case<opcodes::sbc_a_d, tests::get_d>, \
    tests::r8_test_case<opcodes::sbc_a_e, tests::get_e>, \
    tests::r8_test_case<opcodes::sbc_a_h, tests::get_h>, \
    tests::r8_test_case<opcodes::sbc_a_l, tests::get_l>

#define sub_a_r8_test_cases \
    tests::r8_test_case<opcodes::sub_a_b, tests::get_b>, \
    tests::r8_test_case<opcodes::sub_a_c, tests::get_c>, \
    tests::r8_test_case<opcodes::sub_a_d, tests::get_d>, \
    tests::r8_test_case<opcodes::sub_a_e, tests::get_e>, \
    tests::r8_test_case<opcodes::sub_a_h, tests::get_h>, \
    tests::r8_test_case<opcodes::sub_a_l, tests::get_l>
}

TEST_CASE_TEMPLATE("arithmetic.add_a_r8 updates a register properly", test, add_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.reg.a(), 0);

	test::reg(cpu) = 5;
	test::execute(cpu);
	CHECK_EQ(cpu.reg.a(), 5);

	test::reg(cpu) = 125;
	test::execute(cpu);
	CHECK_EQ(cpu.reg.a(), 130);

	test::reg(cpu) = 127;
	test::execute(cpu);
	CHECK_EQ(cpu.reg.a(), 1);
}

TEST_CASE_TEMPLATE("arithmetic.add_a_r8 updates zero flag properly", test, add_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK(cpu.reg.z_flag());

	test::reg(cpu) = 5;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg.z_flag());

	test::reg(cpu) = 251;
	test::execute(cpu);
	CHECK(cpu.reg.z_flag());
}

TEST_CASE_TEMPLATE("arithmetic.add_a_r8 always set substraction flag to zero", test, add_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.n_flag() = true;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg.n_flag());

	cpu.reg.n_flag() = true;
	test::reg(cpu) = 5;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg.n_flag());

	cpu.reg.n_flag() = true;
	test::reg(cpu) = 252;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg.n_flag());
}

TEST_CASE_TEMPLATE("arithmetic.add_a_r8 applies carry flag properly", test, add_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_FALSE(cpu.reg.c_flag());

	test::reg(cpu) = 255;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg.c_flag());

	test::reg(cpu) = 1;
	test::execute(cpu);
	CHECK(cpu.reg.c_flag());

	test::execute(cpu);
	CHECK_FALSE(cpu.reg.c_flag());
}

TEST_CASE_TEMPLATE("arithmetic.add_a_r8 applies half carry flag properly", test, add_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_FALSE(cpu.reg.h_flag());

	test::reg(cpu) = 0xF;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg.h_flag());

	test::execute(cpu);
	CHECK(cpu.reg.h_flag());

	test::reg(cpu) = 0;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg.h_flag());
}

TEST_CASE_TEMPLATE("arithmetic.add_a_r8 increments program counter properly", test, add_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.inc_r8 increments value", test, inc_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 1);
}

TEST_CASE_TEMPLATE("arithmetic.inc_r8 does not increment program counter", test, inc_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.inc_r8 sets flags properly", test, inc_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);

	test::reg(cpu) = 0xFE;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
}

TEST_CASE_TEMPLATE("arithmetic.dec_r8 decrements value", test, dec_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0xFF;
	test::execute(cpu);

	CHECK_EQ(test::reg(cpu), 0xFE);
}

TEST_CASE_TEMPLATE("arithmetic.dec_r8 does not increment program counter", test, dec_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.dec_r8 sets flags properly", test, dec_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), true);

	test::reg(cpu) = 0x2;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
}

TEST_CASE_TEMPLATE("arithmetic.inc_r16 increments value", test, inc_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 1);
}

TEST_CASE_TEMPLATE("arithmetic.inc_r16 does not increment program counter", test, inc_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.inc_r16 does not modify any flags", test, inc_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);

	test::reg(cpu) = 0xFFFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
}

TEST_CASE("arithmetic.cp_a_n8 updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0xBD;
	memory.bus().write(0, 0xFF);

	tests::execute_all_instruction_steps<opcodes::cp_a_n8>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), true);

	cpu.pc = 0;
	cpu.reg.a() = 0xFF;

	tests::execute_all_instruction_steps<opcodes::cp_a_n8>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);
}

TEST_CASE("arithmetic.cp_a_n8 does not modify register a")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	memory.bus().write(0, 0xFF);
	tests::execute_all_instruction_steps<opcodes::cp_a_n8>(cpu);

	CHECK_EQ(cpu.reg.a(), 0);
}

TEST_CASE("arithmetic.cp_a_n8 increments pc properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::cp_a_n8>(cpu);
	CHECK_EQ(cpu.pc, 1);
}

TEST_CASE_TEMPLATE("arithmetic.cp_a_r8 updates flags properly", test, cp_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0xBB;
	test::reg(cpu) = 0xFF;

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), true);

	cpu.reg.a() = 0xFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);
}

TEST_CASE_TEMPLATE("arithmetic.cp_a_r8 does not modify register a", test, cp_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0xFF;
	const cpu::register_8::type_t original_value = cpu.reg.a();

	test::execute(cpu);
	CHECK_EQ(cpu.reg.a(), original_value);
}

TEST_CASE_TEMPLATE("arithmetic.cp_a_r8 does not increment pc", test, cp_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.cp_a_a updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0xBB;
	tests::execute_all_instruction_steps<opcodes::cp_a_a>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.a() = 0xFF;
	tests::execute_all_instruction_steps<opcodes::cp_a_a>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);
}

TEST_CASE("arithmetic.cp_a_a does not modify register a")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0xFF;
	tests::execute_all_instruction_steps<opcodes::cp_a_a>(cpu);

	CHECK_EQ(cpu.reg.a(), 0xFF);
}

TEST_CASE("arithmetic.cp_a_a does not increment pc")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::cp_a_a>(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.add_a_n8 updates a register properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_EQ(cpu.reg.a(), 0);

	cpu.pc = 0;
	memory.bus().write(0, 5);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_EQ(cpu.reg.a(), 5);

	cpu.pc = 0;
	memory.bus().write(0, 125);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_EQ(cpu.reg.a(), 130);

	cpu.pc = 0;
	memory.bus().write(0, 127);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_EQ(cpu.reg.a(), 1);
}

TEST_CASE("arithmetic.add_a_n8 updates zero flag properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK(cpu.reg.z_flag());

	cpu.pc = 0;
	memory.bus().write(0, 5);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.z_flag());

	cpu.pc = 0;
	memory.bus().write(0, 251);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK(cpu.reg.z_flag());
}

TEST_CASE("arithmetic.add_a_n8 always set substraction flag to zero")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.n_flag());

	cpu.pc = 0;
	memory.bus().write(0, 5);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.n_flag());

	cpu.pc = 0;
	memory.bus().write(0, 252);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.n_flag());
}

TEST_CASE("arithmetic.add_a_n8 applies carry flag properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.c_flag());

	cpu.pc = 0;
	memory.bus().write(0, 255);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);
	
	CHECK_FALSE(cpu.reg.c_flag());

	cpu.pc = 0;
	memory.bus().write(0, 1);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK(cpu.reg.c_flag());

	cpu.pc = 0;
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.c_flag());
}

TEST_CASE("arithmetic.add_a_n8 applies half carry flag properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.h_flag());

	cpu.pc = 0;
	memory.bus().write(0, 0xF);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.h_flag());

	cpu.pc = 0;
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK(cpu.reg.h_flag());

	cpu.pc = 0;
	memory.bus().write(0, 0);
	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);

	CHECK_FALSE(cpu.reg.h_flag());
}

TEST_CASE("arithmetic.add_a_n8 increments program counter properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_n8>(cpu);
	CHECK_EQ(cpu.pc, 1);
}

TEST_CASE("arithmetic.sub_a_n8 substracts value properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0xFF;
	memory.bus().write(0, 0xF0);
	tests::execute_all_instruction_steps<opcodes::sub_a_n8>(cpu);

	CHECK_EQ(cpu.reg.a(), 0x0F);
}

TEST_CASE("arithmetic.sub_a_n8 updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0x0F;
	memory.bus().write(0, 0xF0);
	tests::execute_all_instruction_steps<opcodes::sub_a_n8>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), true);

	cpu.pc = 0;
	cpu.reg.a() = 0xF0;
	tests::execute_all_instruction_steps<opcodes::sub_a_n8>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);
}

TEST_CASE("arithmetic.sub_a_n8 increments program counter properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::sub_a_n8>(cpu);
	CHECK_EQ(cpu.pc, 1);
}

TEST_CASE_TEMPLATE("arithmetic.adc_a_r8 updates value properly", test, adc_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.c_flag() = true;

	test::reg(cpu) = 0xFE;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.a(), 0xFF);
}

TEST_CASE_TEMPLATE("arithmetic.adc_a_r8 does not increment program counter", test, adc_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.adc_a_r8 updates flags properly", test, adc_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.a() = 1;
	cpu.reg.c_flag() = false;
	test::reg(cpu) = 0x0F;

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.a() = 1;
	cpu.reg.c_flag() = true;
	test::reg(cpu) = 0xFE;

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), true);
}

TEST_CASE("arithmetic.adc_a_a updates value properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.c_flag() = true;
	cpu.reg.a() = 0x0F;

	tests::execute_all_instruction_steps<opcodes::adc_a_a>(cpu);
	CHECK_EQ(cpu.reg.a(), 0x1F);
}

TEST_CASE("arithmetic.adc_a_a does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::adc_a_a>(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.adc_a_a updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::adc_a_a>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.a() = 0x0F;
	cpu.reg.c_flag() = false;

	tests::execute_all_instruction_steps<opcodes::adc_a_a>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.a() = 0xFF;
	cpu.reg.c_flag() = true;

	tests::execute_all_instruction_steps<opcodes::adc_a_a>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), true);
}

TEST_CASE("arithmetic.adc_a_n8 updates value properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.c_flag() = true;
	memory.bus().write(0, 0x0F);

	tests::execute_all_instruction_steps<opcodes::adc_a_n8>(cpu);
	CHECK_EQ(cpu.reg.a(), 0x10);
}

TEST_CASE("arithmetic.adc_a_n8 increments program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::adc_a_n8>(cpu);
	CHECK_EQ(cpu.pc, 1);
}

TEST_CASE("arithmetic.adc_a_n8 updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::adc_a_n8>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.pc = 0;
	cpu.reg.a() = 0x0F;
	cpu.reg.c_flag() = false;
	memory.bus().write(0, 0x0F);

	tests::execute_all_instruction_steps<opcodes::adc_a_n8>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.pc = 0;
	cpu.reg.a() = 0xFF;
	cpu.reg.c_flag() = true;
	memory.bus().write(0, 0x0F);

	tests::execute_all_instruction_steps<opcodes::adc_a_n8>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), true);
}

TEST_CASE("arithmetic.dec_ind_hl decrements value")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.hl() = 0xABCD;
	memory.bus().write(0xABCD, 0xFF);

	tests::execute_all_instruction_steps<opcodes::dec_ind_hl>(cpu);
	CHECK_EQ(memory.bus().read(0xABCD), 0xFE);
}

TEST_CASE("arithmetic.dec_ind_hl does not increments program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::dec_ind_hl>(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.dec_ind_hl sets flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.hl() = 0xABCD;
	tests::execute_all_instruction_steps<opcodes::dec_ind_hl>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), true);

	memory.bus().write(0xABCD, 0x2);
	tests::execute_all_instruction_steps<opcodes::dec_ind_hl>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);

	tests::execute_all_instruction_steps<opcodes::dec_ind_hl>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
}

TEST_CASE_TEMPLATE("arithmetic.add_hl_r16 updates register hl properly", test, add_hl_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.reg.hl(), 0);

	test::reg(cpu) = 0xF;
	test::execute(cpu);
	CHECK_EQ(cpu.reg.hl(), 0xF);

	test::execute(cpu);
	CHECK_EQ(cpu.reg.hl(), 0x1E);
}

TEST_CASE_TEMPLATE("arithmetic.add_hl_r16 updates flags properly", test, add_hl_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	test::reg(cpu) = 0x0FFF;
	test::execute(cpu);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	test::reg(cpu) = 0x1;
	test::execute(cpu);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), false);

	test::reg(cpu) = 0xFFFF;
	test::execute(cpu);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), true);
}

TEST_CASE_TEMPLATE("arithmetic.add_hl_r16 does not increment program counter", test, add_hl_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.add_hl_hl updates register hl properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_hl_hl>(cpu);

	CHECK_EQ(cpu.reg.hl(), 0);

	cpu.reg.hl() = 0xF;
	tests::execute_all_instruction_steps<opcodes::add_hl_hl>(cpu);

	CHECK_EQ(cpu.reg.hl(), 0x1E);

	tests::execute_all_instruction_steps<opcodes::add_hl_hl>(cpu);

	CHECK_EQ(cpu.reg.hl(), 0x3C);
}

TEST_CASE("arithmetic.add_hl_hl updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_hl_hl>(cpu);

	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.hl() = 0x000F;
	tests::execute_all_instruction_steps<opcodes::add_hl_hl>(cpu);

	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.hl() = 0x0FFF;
	tests::execute_all_instruction_steps<opcodes::add_hl_hl>(cpu);

	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.hl() = 0xEFFF;
	tests::execute_all_instruction_steps<opcodes::add_hl_hl>(cpu);

	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), true);
}

TEST_CASE("arithmetic.add_hl_hl does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_hl_hl>(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.add_hl_sp updates register hl properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_hl_sp>(cpu);
	CHECK_EQ(cpu.reg.hl(), 0);

	cpu.sp = 0xF;
	tests::execute_all_instruction_steps<opcodes::add_hl_sp>(cpu);
	CHECK_EQ(cpu.reg.hl(), 0xF);

	tests::execute_all_instruction_steps<opcodes::add_hl_sp>(cpu);
	CHECK_EQ(cpu.reg.hl(), 0x1E);
}

TEST_CASE("arithmetic.add_hl_sp updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_hl_sp>(cpu);

	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.sp = 0x0FFF;
	tests::execute_all_instruction_steps<opcodes::add_hl_sp>(cpu);

	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.sp = 0x1;
	tests::execute_all_instruction_steps<opcodes::add_hl_sp>(cpu);

	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.sp = 0xFFFF;
	tests::execute_all_instruction_steps<opcodes::add_hl_sp>(cpu);

	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), true);
}

TEST_CASE("arithmetic.add_hl_sp does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_hl_sp>(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.inc_sp does not increment stack pointer")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::inc_sp>(cpu);
	CHECK_EQ(cpu.sp, 1);

	cpu.sp = 0xFFFE;
	tests::execute_all_instruction_steps<opcodes::inc_sp>(cpu);
	CHECK_EQ(cpu.sp, 0xFFFF);

	tests::execute_all_instruction_steps<opcodes::inc_sp>(cpu);
	CHECK_EQ(cpu.sp, 0);
}

TEST_CASE("arithmetic.inc_sp does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::inc_sp>(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.dec_sp decrements stack pointer properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.sp = 0;
	tests::execute_all_instruction_steps<opcodes::dec_sp>(cpu);
	CHECK_EQ(cpu.sp, 0xFFFF);

	tests::execute_all_instruction_steps<opcodes::dec_sp>(cpu);
	CHECK_EQ(cpu.sp, 0xFFFE);

	cpu.sp = 1;
	tests::execute_all_instruction_steps<opcodes::dec_sp>(cpu);
	CHECK_EQ(cpu.sp, 0);
}

TEST_CASE("arithmetic.dec_sp does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::dec_sp>(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.sbc_a_r8 updates register a properly", test, sbc_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0x11;
	cpu.reg.c_flag() = true;

	test::reg(cpu) = 0x1;
	test::execute(cpu);
	CHECK_EQ(cpu.reg.a(), 0x0F);

	test::execute(cpu);
	CHECK_EQ(cpu.reg.a(), 0x0E);
}

TEST_CASE_TEMPLATE("arithmetic.sbc_a_r8 updates flags properly", test, sbc_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0x11;
	cpu.reg.c_flag() = true;

	test::reg(cpu) = 0x1;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), false);

	test::reg(cpu) = 0x0F;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), true);
}

TEST_CASE_TEMPLATE("arithmetic.sbc_a_r8 does not increment program counter", test, sbc_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.dec_r16 decrements value", test, dec_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0x1;
	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xFFFF);
}

TEST_CASE_TEMPLATE("arithmetic.dec_r16 does not increment program counter", test, dec_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.dec_r16 does not modify any flags", test, dec_r16_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.z_flag() = true;
	cpu.reg.n_flag() = true;
	cpu.reg.h_flag() = true;

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), true);

	test::reg(cpu) = 0xFFFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), true);
}

TEST_CASE("arithmetic.add_a_a updates register a properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_a>(cpu);
	CHECK_EQ(cpu.reg.a(), 0);

	cpu.reg.a() = 0xA;
	tests::execute_all_instruction_steps<opcodes::add_a_a>(cpu);
	CHECK_EQ(cpu.reg.a(), 0x14);

	tests::execute_all_instruction_steps<opcodes::add_a_a>(cpu);
	CHECK_EQ(cpu.reg.a(), 0x28);
}

TEST_CASE("arithmetic.add_a_a updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_a>(cpu);
	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.a() = 0x0F;
	tests::execute_all_instruction_steps<opcodes::add_a_a>(cpu);
	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.a() = 0xFF;
	tests::execute_all_instruction_steps<opcodes::add_a_a>(cpu);
	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), false);
	CHECK_EQ(cpu.reg.h_flag(), true);
	CHECK_EQ(cpu.reg.c_flag(), true);
}

TEST_CASE("arithmetic.add_a_a does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::add_a_a>(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE_TEMPLATE("arithmetic.sub_a_r8 substracts value properly", test, sub_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0xFF;
	test::reg(cpu) = 0xF0;

	test::execute(cpu);
	CHECK_EQ(cpu.reg.a(), 0x0F);
}

TEST_CASE_TEMPLATE("arithmetic.sub_a_r8 updates flags properly", test, sub_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0x0F;
	test::reg(cpu) = 0xF0;

	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), false);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), true);

	cpu.reg.a() = 0xF0;
	test::execute(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);
}

TEST_CASE_TEMPLATE("arithmetic.sub_a_r8 does not increment program counter", test, sub_a_r8_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc, 0);
}

TEST_CASE("arithmetic.sub_a_a substracts value properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0xFF;
	tests::execute_all_instruction_steps<opcodes::sub_a_a>(cpu);
	CHECK_EQ(cpu.reg.a(), 0x0);

	cpu.reg.a() = 0x0B;
	tests::execute_all_instruction_steps<opcodes::sub_a_a>(cpu);
	CHECK_EQ(cpu.reg.a(), 0x0);
}

TEST_CASE("arithmetic.sub_a_a updates flags properly")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg.a() = 0x0F;
	tests::execute_all_instruction_steps<opcodes::sub_a_a>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);

	cpu.reg.a() = 0xF0;
	tests::execute_all_instruction_steps<opcodes::sub_a_a>(cpu);

	CHECK_EQ(cpu.reg.z_flag(), true);
	CHECK_EQ(cpu.reg.n_flag(), true);
	CHECK_EQ(cpu.reg.h_flag(), false);
	CHECK_EQ(cpu.reg.c_flag(), false);
}

TEST_CASE("arithmetic.sub_a_a does not increment program counter")
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_instruction_steps<opcodes::sub_a_a>(cpu);
	CHECK_EQ(cpu.pc, 0);
}
