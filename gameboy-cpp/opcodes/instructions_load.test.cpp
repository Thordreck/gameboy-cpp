
#include "doctest.h"

import cpu;
import std;
import opcodes;

namespace
{
	inline auto get_a(cpu::cpu& cpu) { return cpu.registers.a(); }
	inline auto get_b(cpu::cpu& cpu) { return cpu.registers.b(); }
	inline auto get_c(cpu::cpu& cpu) { return cpu.registers.c(); }
	inline auto get_d(cpu::cpu& cpu) { return cpu.registers.d(); }
	inline auto get_e(cpu::cpu& cpu) { return cpu.registers.e(); }
	inline auto get_h(cpu::cpu& cpu) { return cpu.registers.h(); }
	inline auto get_l(cpu::cpu& cpu) { return cpu.registers.l(); }

	template<typename OpCode, auto LhsRegFn, auto RhsRegFn>
	requires requires (cpu::cpu& cpu)
	{
		{ OpCode::execute(cpu) } -> std::same_as<void>;
		{ LhsRegFn(cpu) } -> std::same_as<cpu::register_8>;
		{ RhsRegFn(cpu) } -> std::same_as<cpu::register_8>;
	}
	struct test_pair
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_8 lhs(cpu::cpu& cpu) { return LhsRegFn(cpu); }
		static cpu::register_8 rhs(cpu::cpu& cpu) { return RhsRegFn(cpu); }
	};

	#define test_pairs \
    test_pair<opcodes::ld_a_a, get_a, get_a>, \
    test_pair<opcodes::ld_a_b, get_a, get_b>, \
    test_pair<opcodes::ld_a_c, get_a, get_c>, \
    test_pair<opcodes::ld_a_d, get_a, get_d>, \
    test_pair<opcodes::ld_a_e, get_a, get_e>, \
    test_pair<opcodes::ld_a_h, get_a, get_h>, \
    test_pair<opcodes::ld_a_l, get_a, get_l>, \
    test_pair<opcodes::ld_b_a, get_b, get_a>, \
    test_pair<opcodes::ld_b_b, get_b, get_b>, \
    test_pair<opcodes::ld_b_c, get_b, get_c>, \
    test_pair<opcodes::ld_b_d, get_b, get_d>, \
    test_pair<opcodes::ld_b_e, get_b, get_e>, \
    test_pair<opcodes::ld_b_h, get_b, get_h>, \
    test_pair<opcodes::ld_b_l, get_b, get_l>, \
    test_pair<opcodes::ld_c_a, get_c, get_a>, \
    test_pair<opcodes::ld_c_b, get_c, get_b>, \
    test_pair<opcodes::ld_c_c, get_c, get_c>, \
    test_pair<opcodes::ld_c_d, get_c, get_d>, \
    test_pair<opcodes::ld_c_e, get_c, get_e>, \
    test_pair<opcodes::ld_c_h, get_c, get_h>, \
    test_pair<opcodes::ld_c_l, get_c, get_l>, \
    test_pair<opcodes::ld_d_a, get_d, get_a>, \
    test_pair<opcodes::ld_d_b, get_d, get_b>, \
    test_pair<opcodes::ld_d_c, get_d, get_c>, \
    test_pair<opcodes::ld_d_d, get_d, get_d>, \
    test_pair<opcodes::ld_d_e, get_d, get_e>, \
    test_pair<opcodes::ld_d_h, get_d, get_h>, \
    test_pair<opcodes::ld_d_l, get_d, get_l>, \
    test_pair<opcodes::ld_e_a, get_e, get_a>, \
    test_pair<opcodes::ld_e_b, get_e, get_b>, \
    test_pair<opcodes::ld_e_c, get_e, get_c>, \
    test_pair<opcodes::ld_e_d, get_e, get_d>, \
    test_pair<opcodes::ld_e_e, get_e, get_e>, \
    test_pair<opcodes::ld_e_h, get_e, get_h>, \
    test_pair<opcodes::ld_e_l, get_e, get_l>, \
    test_pair<opcodes::ld_h_a, get_h, get_a>, \
    test_pair<opcodes::ld_h_b, get_h, get_b>, \
    test_pair<opcodes::ld_h_c, get_h, get_c>, \
    test_pair<opcodes::ld_h_d, get_h, get_d>, \
    test_pair<opcodes::ld_h_e, get_h, get_e>, \
    test_pair<opcodes::ld_h_h, get_h, get_h>, \
    test_pair<opcodes::ld_h_l, get_h, get_l>, \
    test_pair<opcodes::ld_l_a, get_l, get_a>, \
    test_pair<opcodes::ld_l_b, get_l, get_b>, \
    test_pair<opcodes::ld_l_c, get_l, get_c>, \
    test_pair<opcodes::ld_l_d, get_l, get_d>, \
    test_pair<opcodes::ld_l_e, get_l, get_e>, \
    test_pair<opcodes::ld_l_h, get_l, get_h>, \
    test_pair<opcodes::ld_l_l, get_l, get_l>
}

TEST_CASE_TEMPLATE("ld_r8_r8 copies registry value and updates pc properly", Opcode, test_pairs)
{
	constexpr cpu::register_8::type_t test_value = 0xAB;

	cpu::cpu cpu{};
	Opcode::rhs(cpu) = test_value;

	Opcode::execute(cpu);
	CHECK_EQ(Opcode::lhs(cpu), test_value);
	CHECK_EQ(cpu.pc.as_bytes(), 1);
}
