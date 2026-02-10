
#include "doctest.h"

import cpu;
import std;
import opcodes;

namespace
{
	template<typename OpCode, auto RegFn>
	requires requires (cpu::cpu& cpu)
	{
		{ OpCode::execute(cpu) } -> std::same_as<void>;
		{ (cpu.registers.*RegFn)() } -> std::same_as<cpu::register_8>;
	}
	struct test_pair
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_8 target_registry(cpu::cpu& cpu)
		{
			return (cpu.registers.*RegFn)();
		}
	};

	#define test_pairs \
    test_pair<opcodes::add_a_b, &cpu::registers::b>, \
    test_pair<opcodes::add_a_c, &cpu::registers::c>, \
    test_pair<opcodes::add_a_d, &cpu::registers::d>, \
    test_pair<opcodes::add_a_e, &cpu::registers::e>, \
    test_pair<opcodes::add_a_h, &cpu::registers::h>, \
    test_pair<opcodes::add_a_l, &cpu::registers::l>
}

TEST_CASE_TEMPLATE("add_a_r8 updates a register properly", Opcode, test_pairs)
{
	cpu::cpu cpu{};

	Opcode::execute(cpu);
	CHECK_EQ(cpu.registers.a(), 0);

	Opcode::target_registry(cpu) = 5;
	Opcode::execute(cpu);
	CHECK_EQ(cpu.registers.a(), 5);

	Opcode::target_registry(cpu) = 125;
	Opcode::execute(cpu);
	CHECK_EQ(cpu.registers.a(), 130);

	Opcode::target_registry(cpu) = 127;
	Opcode::execute(cpu);
	CHECK_EQ(cpu.registers.a(), 1);
}

TEST_CASE_TEMPLATE("add_a_r8 updates zero flag properly", Opcode, test_pairs)
{
	cpu::cpu cpu{};

	Opcode::execute(cpu);
	CHECK(cpu.registers.z_flag());

	Opcode::target_registry(cpu) = 5;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.z_flag());

	Opcode::target_registry(cpu) = 251;
	Opcode::execute(cpu);
	CHECK(cpu.registers.z_flag());
}

TEST_CASE_TEMPLATE("add_a_r8 always set substraction flag to zero", Opcode, test_pairs)
{
	cpu::cpu cpu {};

	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.n_flag());

	Opcode::target_registry(cpu) = 5;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.n_flag());

	Opcode::target_registry(cpu) = 252;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.n_flag());
}

TEST_CASE_TEMPLATE("add_a_r8 applies carry flag properly", Opcode, test_pairs)
{
	cpu::cpu cpu{};

	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.c_flag());

	Opcode::target_registry(cpu) = 255;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.c_flag());

	Opcode::target_registry(cpu) = 1;
	Opcode::execute(cpu);
	CHECK(cpu.registers.c_flag());

	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.c_flag());
}

TEST_CASE_TEMPLATE("add_a_r8 applies half carry flag properly", Opcode, test_pairs)
{
	cpu::cpu cpu{};

	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.h_flag());

	Opcode::target_registry(cpu) = 0xF;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.h_flag());

	Opcode::execute(cpu);
	CHECK(cpu.registers.h_flag());

	Opcode::target_registry(cpu) = 0;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.h_flag());
}

TEST_CASE_TEMPLATE("add_a_r8 increments program counter properly", Opcode, test_pairs)
{
	cpu::cpu cpu{};

	const cpu::program_counter::type_t previous_pc = cpu.pc;
	Opcode::execute(cpu);
	CHECK_EQ(previous_pc + 1, cpu.pc);
}
