
#include "doctest.h"

import cpu;
import std;
import opcodes;

namespace
{
    template <auto RegFn>
    concept R8RegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_8>;
    };

	inline auto get_a(cpu::cpu& cpu) { return cpu.registers.a(); }
	inline auto get_b(cpu::cpu& cpu) { return cpu.registers.b(); }
	inline auto get_c(cpu::cpu& cpu) { return cpu.registers.c(); }
	inline auto get_d(cpu::cpu& cpu) { return cpu.registers.d(); }
	inline auto get_e(cpu::cpu& cpu) { return cpu.registers.e(); }
	inline auto get_h(cpu::cpu& cpu) { return cpu.registers.h(); }
	inline auto get_l(cpu::cpu& cpu) { return cpu.registers.l(); }

	template<opcodes::Instruction OpCode, auto LhsRegFn, auto RhsRegFn>
    requires R8RegisterFetchFn<LhsRegFn> && R8RegisterFetchFn<RhsRegFn>
	struct ld_r8_r8_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_8 lhs(cpu::cpu& cpu) { return LhsRegFn(cpu); }
		static cpu::register_8 rhs(cpu::cpu& cpu) { return RhsRegFn(cpu); }
	};

	#define ld_r8_r8_test_cases \
    ld_r8_r8_test_case<opcodes::ld_a_a, get_a, get_a>, \
    ld_r8_r8_test_case<opcodes::ld_a_b, get_a, get_b>, \
    ld_r8_r8_test_case<opcodes::ld_a_c, get_a, get_c>, \
    ld_r8_r8_test_case<opcodes::ld_a_d, get_a, get_d>, \
    ld_r8_r8_test_case<opcodes::ld_a_e, get_a, get_e>, \
    ld_r8_r8_test_case<opcodes::ld_a_h, get_a, get_h>, \
    ld_r8_r8_test_case<opcodes::ld_a_l, get_a, get_l>, \
    ld_r8_r8_test_case<opcodes::ld_b_a, get_b, get_a>, \
    ld_r8_r8_test_case<opcodes::ld_b_b, get_b, get_b>, \
    ld_r8_r8_test_case<opcodes::ld_b_c, get_b, get_c>, \
    ld_r8_r8_test_case<opcodes::ld_b_d, get_b, get_d>, \
    ld_r8_r8_test_case<opcodes::ld_b_e, get_b, get_e>, \
    ld_r8_r8_test_case<opcodes::ld_b_h, get_b, get_h>, \
    ld_r8_r8_test_case<opcodes::ld_b_l, get_b, get_l>, \
    ld_r8_r8_test_case<opcodes::ld_c_a, get_c, get_a>, \
    ld_r8_r8_test_case<opcodes::ld_c_b, get_c, get_b>, \
    ld_r8_r8_test_case<opcodes::ld_c_c, get_c, get_c>, \
    ld_r8_r8_test_case<opcodes::ld_c_d, get_c, get_d>, \
    ld_r8_r8_test_case<opcodes::ld_c_e, get_c, get_e>, \
    ld_r8_r8_test_case<opcodes::ld_c_h, get_c, get_h>, \
    ld_r8_r8_test_case<opcodes::ld_c_l, get_c, get_l>, \
    ld_r8_r8_test_case<opcodes::ld_d_a, get_d, get_a>, \
    ld_r8_r8_test_case<opcodes::ld_d_b, get_d, get_b>, \
    ld_r8_r8_test_case<opcodes::ld_d_c, get_d, get_c>, \
    ld_r8_r8_test_case<opcodes::ld_d_d, get_d, get_d>, \
    ld_r8_r8_test_case<opcodes::ld_d_e, get_d, get_e>, \
    ld_r8_r8_test_case<opcodes::ld_d_h, get_d, get_h>, \
    ld_r8_r8_test_case<opcodes::ld_d_l, get_d, get_l>, \
    ld_r8_r8_test_case<opcodes::ld_e_a, get_e, get_a>, \
    ld_r8_r8_test_case<opcodes::ld_e_b, get_e, get_b>, \
    ld_r8_r8_test_case<opcodes::ld_e_c, get_e, get_c>, \
    ld_r8_r8_test_case<opcodes::ld_e_d, get_e, get_d>, \
    ld_r8_r8_test_case<opcodes::ld_e_e, get_e, get_e>, \
    ld_r8_r8_test_case<opcodes::ld_e_h, get_e, get_h>, \
    ld_r8_r8_test_case<opcodes::ld_e_l, get_e, get_l>, \
    ld_r8_r8_test_case<opcodes::ld_h_a, get_h, get_a>, \
    ld_r8_r8_test_case<opcodes::ld_h_b, get_h, get_b>, \
    ld_r8_r8_test_case<opcodes::ld_h_c, get_h, get_c>, \
    ld_r8_r8_test_case<opcodes::ld_h_d, get_h, get_d>, \
    ld_r8_r8_test_case<opcodes::ld_h_e, get_h, get_e>, \
    ld_r8_r8_test_case<opcodes::ld_h_h, get_h, get_h>, \
    ld_r8_r8_test_case<opcodes::ld_h_l, get_h, get_l>, \
    ld_r8_r8_test_case<opcodes::ld_l_a, get_l, get_a>, \
    ld_r8_r8_test_case<opcodes::ld_l_b, get_l, get_b>, \
    ld_r8_r8_test_case<opcodes::ld_l_c, get_l, get_c>, \
    ld_r8_r8_test_case<opcodes::ld_l_d, get_l, get_d>, \
    ld_r8_r8_test_case<opcodes::ld_l_e, get_l, get_e>, \
    ld_r8_r8_test_case<opcodes::ld_l_h, get_l, get_h>, \
    ld_r8_r8_test_case<opcodes::ld_l_l, get_l, get_l>

	template<opcodes::Instruction OpCode, auto RegFn>
    requires R8RegisterFetchFn<RegFn>
	struct ld_r8_n8_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_8 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	#define ld_r8_n8_test_cases \
    ld_r8_n8_test_case<opcodes::ld_a_n8, get_a>, \
    ld_r8_n8_test_case<opcodes::ld_b_n8, get_b>, \
    ld_r8_n8_test_case<opcodes::ld_c_n8, get_c>, \
    ld_r8_n8_test_case<opcodes::ld_d_n8, get_d>, \
    ld_r8_n8_test_case<opcodes::ld_e_n8, get_e>, \
    ld_r8_n8_test_case<opcodes::ld_h_n8, get_h>, \
    ld_r8_n8_test_case<opcodes::ld_l_n8, get_l>
}

TEST_CASE_TEMPLATE("ld_r8_r8 copies registry value and updates pc properly", Opcode, ld_r8_r8_test_cases)
{
	constexpr cpu::register_8::type_t test_value = 0xAB;

	cpu::cpu cpu{};
	Opcode::rhs(cpu) = test_value;

	Opcode::execute(cpu);
	CHECK_EQ(Opcode::lhs(cpu), test_value);
	CHECK_EQ(cpu.pc.as_bytes(), 1);
}

TEST_CASE_TEMPLATE("ld_r8_n8 stores value into target registry and updates pc properly", Opcode, ld_r8_n8_test_cases)
{
	constexpr cpu::register_8::type_t test_value = 0xAB;

	cpu::cpu cpu{};
    cpu.memory[1] = test_value;
	Opcode::execute(cpu);

	CHECK_EQ(Opcode::reg(cpu), test_value);
	CHECK_EQ(cpu.pc.as_bytes(), 2);
}
