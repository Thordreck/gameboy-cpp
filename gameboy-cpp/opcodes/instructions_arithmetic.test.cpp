
#include "doctest.h"

import cpu;
import std;
import opcodes;

namespace
{
	inline auto get_b(cpu::cpu& cpu) { return cpu.registers.b(); }
	inline auto get_c(cpu::cpu& cpu) { return cpu.registers.c(); }
	inline auto get_d(cpu::cpu& cpu) { return cpu.registers.d(); }
	inline auto get_e(cpu::cpu& cpu) { return cpu.registers.e(); }
	inline auto get_h(cpu::cpu& cpu) { return cpu.registers.h(); }
	inline auto get_l(cpu::cpu& cpu) { return cpu.registers.l(); }

	template<typename OpCode, auto RegFn>
	requires requires (cpu::cpu& cpu)
	{
		{ OpCode::execute(cpu) } -> std::same_as<void>;
		{ RegFn(cpu) } -> std::same_as<cpu::register_8>;
	}
	struct test_pair
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_8 target_registry(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	#define test_pairs \
    test_pair<opcodes::add_a_b, get_b>, \
    test_pair<opcodes::add_a_c, get_c>, \
    test_pair<opcodes::add_a_d, get_d>, \
    test_pair<opcodes::add_a_e, get_e>, \
    test_pair<opcodes::add_a_h, get_h>, \
    test_pair<opcodes::add_a_l, get_l>
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

	const cpu::program_counter previous_pc = cpu.pc;
	Opcode::execute(cpu);
	CHECK_EQ(previous_pc + 1, cpu.pc);
}
