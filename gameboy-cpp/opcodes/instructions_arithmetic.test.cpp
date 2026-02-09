
#include "doctest.h"

import cpu;
import std;
import opcodes;

namespace
{
	template<typename Opcode>
	cpu::register_8 add_a_r8_registry(cpu::cpu& cpu)
	{
		return (cpu.registers.*Opcode::reg_fn)();
	}
}

TEST_CASE_TEMPLATE(
	"add_a_r8 updates a register properly", 
	Opcode, 
	opcodes::add_a_b, 
	opcodes::add_a_c, 
	opcodes::add_a_d, 
	opcodes::add_a_e, 
	opcodes::add_a_h, 
	opcodes::add_a_l)
{
	cpu::cpu cpu{};

	Opcode::execute(cpu);
	CHECK_EQ(cpu.registers.a(), 0);

	add_a_r8_registry<Opcode>(cpu) = 5;
	Opcode::execute(cpu);
	CHECK_EQ(cpu.registers.a(), 5);

	add_a_r8_registry<Opcode>(cpu) = 125;
	Opcode::execute(cpu);
	CHECK_EQ(cpu.registers.a(), 130);

	add_a_r8_registry<Opcode>(cpu) = 127;
	Opcode::execute(cpu);
	CHECK_EQ(cpu.registers.a(), 1);
}

TEST_CASE_TEMPLATE(
	"add_a_r8 updates zero flag properly",
	Opcode, 
	opcodes::add_a_b, 
	opcodes::add_a_c, 
	opcodes::add_a_d, 
	opcodes::add_a_e, 
	opcodes::add_a_h, 
	opcodes::add_a_l)
{
	cpu::cpu cpu{};

	Opcode::execute(cpu);
	CHECK(cpu.registers.z_flag());

	add_a_r8_registry<Opcode>(cpu) = 5;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.z_flag());

	add_a_r8_registry<Opcode>(cpu) = 251;
	Opcode::execute(cpu);
	CHECK(cpu.registers.z_flag());
}

TEST_CASE_TEMPLATE(
	"add_a_r8 always set substraction flag to zero",
	Opcode, 
	opcodes::add_a_b, 
	opcodes::add_a_c, 
	opcodes::add_a_d, 
	opcodes::add_a_e, 
	opcodes::add_a_h, 
	opcodes::add_a_l)
{
	cpu::cpu cpu {};

	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.n_flag());

	add_a_r8_registry<Opcode>(cpu) = 5;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.n_flag());

	add_a_r8_registry<Opcode>(cpu) = 252;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.n_flag());
}

TEST_CASE_TEMPLATE(
	"add_a_r8 applies carry flag properly",
	Opcode, 
	opcodes::add_a_b, 
	opcodes::add_a_c, 
	opcodes::add_a_d, 
	opcodes::add_a_e, 
	opcodes::add_a_h, 
	opcodes::add_a_l)
{
	cpu::cpu cpu{};

	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.c_flag());

	add_a_r8_registry<Opcode>(cpu) = 255;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.c_flag());

	add_a_r8_registry<Opcode>(cpu) = 1;
	Opcode::execute(cpu);
	CHECK(cpu.registers.c_flag());

	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.c_flag());
}

TEST_CASE_TEMPLATE(
	"add_a_r8 applies half carry flag properly",
	Opcode, 
	opcodes::add_a_b, 
	opcodes::add_a_c, 
	opcodes::add_a_d, 
	opcodes::add_a_e, 
	opcodes::add_a_h, 
	opcodes::add_a_l)
{
	cpu::cpu cpu{};

	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.h_flag());

	add_a_r8_registry<Opcode>(cpu) = 0xF;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.h_flag());

	Opcode::execute(cpu);
	CHECK(cpu.registers.h_flag());

	add_a_r8_registry<Opcode>(cpu) = 0;
	Opcode::execute(cpu);
	CHECK_FALSE(cpu.registers.h_flag());
}
