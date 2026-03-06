
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

namespace
{
	#define or_a_r8_test_cases \
    tests::r8_test_case<opcodes::or_a_b, tests::get_b>, \
    tests::r8_test_case<opcodes::or_a_c, tests::get_c>, \
    tests::r8_test_case<opcodes::or_a_d, tests::get_d>, \
    tests::r8_test_case<opcodes::or_a_e, tests::get_e>, \
    tests::r8_test_case<opcodes::or_a_h, tests::get_h>, \
    tests::r8_test_case<opcodes::or_a_l, tests::get_l>

	#define xor_a_r8_test_cases \
    tests::r8_test_case<opcodes::xor_a_b, tests::get_b>, \
    tests::r8_test_case<opcodes::xor_a_c, tests::get_c>, \
    tests::r8_test_case<opcodes::xor_a_d, tests::get_d>, \
    tests::r8_test_case<opcodes::xor_a_e, tests::get_e>, \
    tests::r8_test_case<opcodes::xor_a_h, tests::get_h>, \
    tests::r8_test_case<opcodes::xor_a_l, tests::get_l>

	#define srl_r8_test_cases \
    tests::r8_test_case<opcodes::srl_a, tests::get_a>, \
    tests::r8_test_case<opcodes::srl_b, tests::get_b>, \
    tests::r8_test_case<opcodes::srl_c, tests::get_c>, \
    tests::r8_test_case<opcodes::srl_d, tests::get_d>, \
    tests::r8_test_case<opcodes::srl_e, tests::get_e>, \
    tests::r8_test_case<opcodes::srl_h, tests::get_h>, \
    tests::r8_test_case<opcodes::srl_l, tests::get_l>

	#define rr_r8_test_cases \
    tests::r8_test_case<opcodes::rr_a, tests::get_a>, \
    tests::r8_test_case<opcodes::rr_b, tests::get_b>, \
    tests::r8_test_case<opcodes::rr_c, tests::get_c>, \
    tests::r8_test_case<opcodes::rr_d, tests::get_d>, \
    tests::r8_test_case<opcodes::rr_e, tests::get_e>, \
    tests::r8_test_case<opcodes::rr_h, tests::get_h>, \
    tests::r8_test_case<opcodes::rr_l, tests::get_l>

	#define bit_u3_r8_test_cases \
    tests::r8_u3_test_case<opcodes::bit_0_a, tests::get_a, 0>, \
    tests::r8_u3_test_case<opcodes::bit_1_a, tests::get_a, 1>, \
    tests::r8_u3_test_case<opcodes::bit_2_a, tests::get_a, 2>, \
    tests::r8_u3_test_case<opcodes::bit_3_a, tests::get_a, 3>, \
    tests::r8_u3_test_case<opcodes::bit_4_a, tests::get_a, 4>, \
    tests::r8_u3_test_case<opcodes::bit_5_a, tests::get_a, 5>, \
    tests::r8_u3_test_case<opcodes::bit_6_a, tests::get_a, 6>, \
    tests::r8_u3_test_case<opcodes::bit_7_a, tests::get_a, 7>, \
    tests::r8_u3_test_case<opcodes::bit_0_b, tests::get_b, 0>, \
    tests::r8_u3_test_case<opcodes::bit_1_b, tests::get_b, 1>, \
    tests::r8_u3_test_case<opcodes::bit_2_b, tests::get_b, 2>, \
    tests::r8_u3_test_case<opcodes::bit_3_b, tests::get_b, 3>, \
    tests::r8_u3_test_case<opcodes::bit_4_b, tests::get_b, 4>, \
    tests::r8_u3_test_case<opcodes::bit_5_b, tests::get_b, 5>, \
    tests::r8_u3_test_case<opcodes::bit_6_b, tests::get_b, 6>, \
    tests::r8_u3_test_case<opcodes::bit_7_b, tests::get_b, 7>, \
    tests::r8_u3_test_case<opcodes::bit_0_c, tests::get_c, 0>, \
    tests::r8_u3_test_case<opcodes::bit_1_c, tests::get_c, 1>, \
    tests::r8_u3_test_case<opcodes::bit_2_c, tests::get_c, 2>, \
    tests::r8_u3_test_case<opcodes::bit_3_c, tests::get_c, 3>, \
    tests::r8_u3_test_case<opcodes::bit_4_c, tests::get_c, 4>, \
    tests::r8_u3_test_case<opcodes::bit_5_c, tests::get_c, 5>, \
    tests::r8_u3_test_case<opcodes::bit_6_c, tests::get_c, 6>, \
    tests::r8_u3_test_case<opcodes::bit_7_c, tests::get_c, 7>, \
    tests::r8_u3_test_case<opcodes::bit_0_d, tests::get_d, 0>, \
    tests::r8_u3_test_case<opcodes::bit_1_d, tests::get_d, 1>, \
    tests::r8_u3_test_case<opcodes::bit_2_d, tests::get_d, 2>, \
    tests::r8_u3_test_case<opcodes::bit_3_d, tests::get_d, 3>, \
    tests::r8_u3_test_case<opcodes::bit_4_d, tests::get_d, 4>, \
    tests::r8_u3_test_case<opcodes::bit_5_d, tests::get_d, 5>, \
    tests::r8_u3_test_case<opcodes::bit_6_d, tests::get_d, 6>, \
    tests::r8_u3_test_case<opcodes::bit_7_d, tests::get_d, 7>, \
    tests::r8_u3_test_case<opcodes::bit_0_e, tests::get_e, 0>, \
    tests::r8_u3_test_case<opcodes::bit_1_e, tests::get_e, 1>, \
    tests::r8_u3_test_case<opcodes::bit_2_e, tests::get_e, 2>, \
    tests::r8_u3_test_case<opcodes::bit_3_e, tests::get_e, 3>, \
    tests::r8_u3_test_case<opcodes::bit_4_e, tests::get_e, 4>, \
    tests::r8_u3_test_case<opcodes::bit_5_e, tests::get_e, 5>, \
    tests::r8_u3_test_case<opcodes::bit_6_e, tests::get_e, 6>, \
    tests::r8_u3_test_case<opcodes::bit_7_e, tests::get_e, 7>, \
    tests::r8_u3_test_case<opcodes::bit_0_h, tests::get_h, 0>, \
    tests::r8_u3_test_case<opcodes::bit_1_h, tests::get_h, 1>, \
    tests::r8_u3_test_case<opcodes::bit_2_h, tests::get_h, 2>, \
    tests::r8_u3_test_case<opcodes::bit_3_h, tests::get_h, 3>, \
    tests::r8_u3_test_case<opcodes::bit_4_h, tests::get_h, 4>, \
    tests::r8_u3_test_case<opcodes::bit_5_h, tests::get_h, 5>, \
    tests::r8_u3_test_case<opcodes::bit_6_h, tests::get_h, 6>, \
    tests::r8_u3_test_case<opcodes::bit_7_h, tests::get_h, 7>, \
    tests::r8_u3_test_case<opcodes::bit_0_l, tests::get_l, 0>, \
    tests::r8_u3_test_case<opcodes::bit_1_l, tests::get_l, 1>, \
    tests::r8_u3_test_case<opcodes::bit_2_l, tests::get_l, 2>, \
    tests::r8_u3_test_case<opcodes::bit_3_l, tests::get_l, 3>, \
    tests::r8_u3_test_case<opcodes::bit_4_l, tests::get_l, 4>, \
    tests::r8_u3_test_case<opcodes::bit_5_l, tests::get_l, 5>, \
    tests::r8_u3_test_case<opcodes::bit_6_l, tests::get_l, 6>, \
    tests::r8_u3_test_case<opcodes::bit_7_l, tests::get_l, 7>

	#define res_u3_r8_test_cases \
    tests::r8_u3_test_case<opcodes::res_0_a, tests::get_a, 0>, \
    tests::r8_u3_test_case<opcodes::res_1_a, tests::get_a, 1>, \
    tests::r8_u3_test_case<opcodes::res_2_a, tests::get_a, 2>, \
    tests::r8_u3_test_case<opcodes::res_3_a, tests::get_a, 3>, \
    tests::r8_u3_test_case<opcodes::res_4_a, tests::get_a, 4>, \
    tests::r8_u3_test_case<opcodes::res_5_a, tests::get_a, 5>, \
    tests::r8_u3_test_case<opcodes::res_6_a, tests::get_a, 6>, \
    tests::r8_u3_test_case<opcodes::res_7_a, tests::get_a, 7>, \
    tests::r8_u3_test_case<opcodes::res_0_b, tests::get_b, 0>, \
    tests::r8_u3_test_case<opcodes::res_1_b, tests::get_b, 1>, \
    tests::r8_u3_test_case<opcodes::res_2_b, tests::get_b, 2>, \
    tests::r8_u3_test_case<opcodes::res_3_b, tests::get_b, 3>, \
    tests::r8_u3_test_case<opcodes::res_4_b, tests::get_b, 4>, \
    tests::r8_u3_test_case<opcodes::res_5_b, tests::get_b, 5>, \
    tests::r8_u3_test_case<opcodes::res_6_b, tests::get_b, 6>, \
    tests::r8_u3_test_case<opcodes::res_7_b, tests::get_b, 7>, \
    tests::r8_u3_test_case<opcodes::res_0_c, tests::get_c, 0>, \
    tests::r8_u3_test_case<opcodes::res_1_c, tests::get_c, 1>, \
    tests::r8_u3_test_case<opcodes::res_2_c, tests::get_c, 2>, \
    tests::r8_u3_test_case<opcodes::res_3_c, tests::get_c, 3>, \
    tests::r8_u3_test_case<opcodes::res_4_c, tests::get_c, 4>, \
    tests::r8_u3_test_case<opcodes::res_5_c, tests::get_c, 5>, \
    tests::r8_u3_test_case<opcodes::res_6_c, tests::get_c, 6>, \
    tests::r8_u3_test_case<opcodes::res_7_c, tests::get_c, 7>, \
    tests::r8_u3_test_case<opcodes::res_0_d, tests::get_d, 0>, \
    tests::r8_u3_test_case<opcodes::res_1_d, tests::get_d, 1>, \
    tests::r8_u3_test_case<opcodes::res_2_d, tests::get_d, 2>, \
    tests::r8_u3_test_case<opcodes::res_3_d, tests::get_d, 3>, \
    tests::r8_u3_test_case<opcodes::res_4_d, tests::get_d, 4>, \
    tests::r8_u3_test_case<opcodes::res_5_d, tests::get_d, 5>, \
    tests::r8_u3_test_case<opcodes::res_6_d, tests::get_d, 6>, \
    tests::r8_u3_test_case<opcodes::res_7_d, tests::get_d, 7>, \
    tests::r8_u3_test_case<opcodes::res_0_e, tests::get_e, 0>, \
    tests::r8_u3_test_case<opcodes::res_1_e, tests::get_e, 1>, \
    tests::r8_u3_test_case<opcodes::res_2_e, tests::get_e, 2>, \
    tests::r8_u3_test_case<opcodes::res_3_e, tests::get_e, 3>, \
    tests::r8_u3_test_case<opcodes::res_4_e, tests::get_e, 4>, \
    tests::r8_u3_test_case<opcodes::res_5_e, tests::get_e, 5>, \
    tests::r8_u3_test_case<opcodes::res_6_e, tests::get_e, 6>, \
    tests::r8_u3_test_case<opcodes::res_7_e, tests::get_e, 7>, \
    tests::r8_u3_test_case<opcodes::res_0_h, tests::get_h, 0>, \
    tests::r8_u3_test_case<opcodes::res_1_h, tests::get_h, 1>, \
    tests::r8_u3_test_case<opcodes::res_2_h, tests::get_h, 2>, \
    tests::r8_u3_test_case<opcodes::res_3_h, tests::get_h, 3>, \
    tests::r8_u3_test_case<opcodes::res_4_h, tests::get_h, 4>, \
    tests::r8_u3_test_case<opcodes::res_5_h, tests::get_h, 5>, \
    tests::r8_u3_test_case<opcodes::res_6_h, tests::get_h, 6>, \
    tests::r8_u3_test_case<opcodes::res_7_h, tests::get_h, 7>, \
    tests::r8_u3_test_case<opcodes::res_0_l, tests::get_l, 0>, \
    tests::r8_u3_test_case<opcodes::res_1_l, tests::get_l, 1>, \
    tests::r8_u3_test_case<opcodes::res_2_l, tests::get_l, 2>, \
    tests::r8_u3_test_case<opcodes::res_3_l, tests::get_l, 3>, \
    tests::r8_u3_test_case<opcodes::res_4_l, tests::get_l, 4>, \
    tests::r8_u3_test_case<opcodes::res_5_l, tests::get_l, 5>, \
    tests::r8_u3_test_case<opcodes::res_6_l, tests::get_l, 6>, \
    tests::r8_u3_test_case<opcodes::res_7_l, tests::get_l, 7>

	#define set_u3_r8_test_cases \
    tests::r8_u3_test_case<opcodes::set_0_a, tests::get_a, 0>, \
    tests::r8_u3_test_case<opcodes::set_1_a, tests::get_a, 1>, \
    tests::r8_u3_test_case<opcodes::set_2_a, tests::get_a, 2>, \
    tests::r8_u3_test_case<opcodes::set_3_a, tests::get_a, 3>, \
    tests::r8_u3_test_case<opcodes::set_4_a, tests::get_a, 4>, \
    tests::r8_u3_test_case<opcodes::set_5_a, tests::get_a, 5>, \
    tests::r8_u3_test_case<opcodes::set_6_a, tests::get_a, 6>, \
    tests::r8_u3_test_case<opcodes::set_7_a, tests::get_a, 7>, \
    tests::r8_u3_test_case<opcodes::set_0_b, tests::get_b, 0>, \
    tests::r8_u3_test_case<opcodes::set_1_b, tests::get_b, 1>, \
    tests::r8_u3_test_case<opcodes::set_2_b, tests::get_b, 2>, \
    tests::r8_u3_test_case<opcodes::set_3_b, tests::get_b, 3>, \
    tests::r8_u3_test_case<opcodes::set_4_b, tests::get_b, 4>, \
    tests::r8_u3_test_case<opcodes::set_5_b, tests::get_b, 5>, \
    tests::r8_u3_test_case<opcodes::set_6_b, tests::get_b, 6>, \
    tests::r8_u3_test_case<opcodes::set_7_b, tests::get_b, 7>, \
    tests::r8_u3_test_case<opcodes::set_0_c, tests::get_c, 0>, \
    tests::r8_u3_test_case<opcodes::set_1_c, tests::get_c, 1>, \
    tests::r8_u3_test_case<opcodes::set_2_c, tests::get_c, 2>, \
    tests::r8_u3_test_case<opcodes::set_3_c, tests::get_c, 3>, \
    tests::r8_u3_test_case<opcodes::set_4_c, tests::get_c, 4>, \
    tests::r8_u3_test_case<opcodes::set_5_c, tests::get_c, 5>, \
    tests::r8_u3_test_case<opcodes::set_6_c, tests::get_c, 6>, \
    tests::r8_u3_test_case<opcodes::set_7_c, tests::get_c, 7>, \
    tests::r8_u3_test_case<opcodes::set_0_d, tests::get_d, 0>, \
    tests::r8_u3_test_case<opcodes::set_1_d, tests::get_d, 1>, \
    tests::r8_u3_test_case<opcodes::set_2_d, tests::get_d, 2>, \
    tests::r8_u3_test_case<opcodes::set_3_d, tests::get_d, 3>, \
    tests::r8_u3_test_case<opcodes::set_4_d, tests::get_d, 4>, \
    tests::r8_u3_test_case<opcodes::set_5_d, tests::get_d, 5>, \
    tests::r8_u3_test_case<opcodes::set_6_d, tests::get_d, 6>, \
    tests::r8_u3_test_case<opcodes::set_7_d, tests::get_d, 7>, \
    tests::r8_u3_test_case<opcodes::set_0_e, tests::get_e, 0>, \
    tests::r8_u3_test_case<opcodes::set_1_e, tests::get_e, 1>, \
    tests::r8_u3_test_case<opcodes::set_2_e, tests::get_e, 2>, \
    tests::r8_u3_test_case<opcodes::set_3_e, tests::get_e, 3>, \
    tests::r8_u3_test_case<opcodes::set_4_e, tests::get_e, 4>, \
    tests::r8_u3_test_case<opcodes::set_5_e, tests::get_e, 5>, \
    tests::r8_u3_test_case<opcodes::set_6_e, tests::get_e, 6>, \
    tests::r8_u3_test_case<opcodes::set_7_e, tests::get_e, 7>, \
    tests::r8_u3_test_case<opcodes::set_0_h, tests::get_h, 0>, \
    tests::r8_u3_test_case<opcodes::set_1_h, tests::get_h, 1>, \
    tests::r8_u3_test_case<opcodes::set_2_h, tests::get_h, 2>, \
    tests::r8_u3_test_case<opcodes::set_3_h, tests::get_h, 3>, \
    tests::r8_u3_test_case<opcodes::set_4_h, tests::get_h, 4>, \
    tests::r8_u3_test_case<opcodes::set_5_h, tests::get_h, 5>, \
    tests::r8_u3_test_case<opcodes::set_6_h, tests::get_h, 6>, \
    tests::r8_u3_test_case<opcodes::set_7_h, tests::get_h, 7>, \
    tests::r8_u3_test_case<opcodes::set_0_l, tests::get_l, 0>, \
    tests::r8_u3_test_case<opcodes::set_1_l, tests::get_l, 1>, \
    tests::r8_u3_test_case<opcodes::set_2_l, tests::get_l, 2>, \
    tests::r8_u3_test_case<opcodes::set_3_l, tests::get_l, 3>, \
    tests::r8_u3_test_case<opcodes::set_4_l, tests::get_l, 4>, \
    tests::r8_u3_test_case<opcodes::set_5_l, tests::get_l, 5>, \
    tests::r8_u3_test_case<opcodes::set_6_l, tests::get_l, 6>, \
    tests::r8_u3_test_case<opcodes::set_7_l, tests::get_l, 7>

	#define swap_r8_test_cases \
    tests::r8_test_case<opcodes::swap_a, tests::get_a>, \
    tests::r8_test_case<opcodes::swap_b, tests::get_b>, \
    tests::r8_test_case<opcodes::swap_c, tests::get_c>, \
    tests::r8_test_case<opcodes::swap_d, tests::get_d>, \
    tests::r8_test_case<opcodes::swap_e, tests::get_e>, \
    tests::r8_test_case<opcodes::swap_h, tests::get_h>, \
    tests::r8_test_case<opcodes::swap_l, tests::get_l>

	#define and_a_r8_test_cases \
    tests::r8_test_case<opcodes::and_a_b, tests::get_b>, \
    tests::r8_test_case<opcodes::and_a_c, tests::get_c>, \
    tests::r8_test_case<opcodes::and_a_d, tests::get_d>, \
    tests::r8_test_case<opcodes::and_a_e, tests::get_e>, \
    tests::r8_test_case<opcodes::and_a_h, tests::get_h>, \
    tests::r8_test_case<opcodes::and_a_l, tests::get_l>

	#define prefix_rlc_r8_test_cases \
    tests::r8_test_case<opcodes::prefix_rlc_a, tests::get_a>, \
    tests::r8_test_case<opcodes::prefix_rlc_b, tests::get_b>, \
    tests::r8_test_case<opcodes::prefix_rlc_c, tests::get_c>, \
    tests::r8_test_case<opcodes::prefix_rlc_d, tests::get_d>, \
    tests::r8_test_case<opcodes::prefix_rlc_e, tests::get_e>, \
    tests::r8_test_case<opcodes::prefix_rlc_h, tests::get_h>, \
    tests::r8_test_case<opcodes::prefix_rlc_l, tests::get_l>

	#define prefix_rrc_r8_test_cases \
    tests::r8_test_case<opcodes::prefix_rrc_a, tests::get_a>, \
    tests::r8_test_case<opcodes::prefix_rrc_b, tests::get_b>, \
    tests::r8_test_case<opcodes::prefix_rrc_c, tests::get_c>, \
    tests::r8_test_case<opcodes::prefix_rrc_d, tests::get_d>, \
    tests::r8_test_case<opcodes::prefix_rrc_e, tests::get_e>, \
    tests::r8_test_case<opcodes::prefix_rrc_h, tests::get_h>, \
    tests::r8_test_case<opcodes::prefix_rrc_l, tests::get_l>

	#define prefix_rl_r8_test_cases \
    tests::r8_test_case<opcodes::prefix_rl_a, tests::get_a>, \
    tests::r8_test_case<opcodes::prefix_rl_b, tests::get_b>, \
    tests::r8_test_case<opcodes::prefix_rl_c, tests::get_c>, \
    tests::r8_test_case<opcodes::prefix_rl_d, tests::get_d>, \
    tests::r8_test_case<opcodes::prefix_rl_e, tests::get_e>, \
    tests::r8_test_case<opcodes::prefix_rl_h, tests::get_h>, \
    tests::r8_test_case<opcodes::prefix_rl_l, tests::get_l>

	#define sla_r8_test_cases \
    tests::r8_test_case<opcodes::sla_a, tests::get_a>, \
    tests::r8_test_case<opcodes::sla_b, tests::get_b>, \
    tests::r8_test_case<opcodes::sla_c, tests::get_c>, \
    tests::r8_test_case<opcodes::sla_d, tests::get_d>, \
    tests::r8_test_case<opcodes::sla_e, tests::get_e>, \
    tests::r8_test_case<opcodes::sla_h, tests::get_h>, \
    tests::r8_test_case<opcodes::sla_l, tests::get_l>

	#define sra_r8_test_cases \
    tests::r8_test_case<opcodes::sra_a, tests::get_a>, \
    tests::r8_test_case<opcodes::sra_b, tests::get_b>, \
    tests::r8_test_case<opcodes::sra_c, tests::get_c>, \
    tests::r8_test_case<opcodes::sra_d, tests::get_d>, \
    tests::r8_test_case<opcodes::sra_e, tests::get_e>, \
    tests::r8_test_case<opcodes::sra_h, tests::get_h>, \
    tests::r8_test_case<opcodes::sra_l, tests::get_l>
}

TEST_CASE_TEMPLATE("bits.or_a_r8 updates a register properly", test, or_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xF0;

	test::reg(cpu) = 0x0F;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xFF);
}

TEST_CASE_TEMPLATE("bits.or_a_r8 updates program counter properly", test, or_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.or_a_r8 updates flags properly", test, or_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;

	test::reg(cpu) = 0x00;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::reg(cpu) = 0x0F;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.or_a_a updates a register properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xF0;
	tests::execute_all_machine_cycles<opcodes::or_a_a>(cpu);

	CHECK_EQ(cpu.reg().a(), 0xF0);
}

TEST_CASE("bits.or_a_a updates program counter properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::or_a_a>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.or_a_a updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	tests::execute_all_machine_cycles<opcodes::or_a_a>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0x0F;
	tests::execute_all_machine_cycles<opcodes::or_a_a>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.and_a_n8 increments program counter properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::and_a_n8>(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("bits.and_a_n8 updates register a properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	memory.bus().write(0, 0x01);

	tests::execute_all_machine_cycles<opcodes::and_a_n8>(cpu);
	CHECK_EQ(cpu.reg().a(), 0);

	cpu.reg().a() = 0xFF;
	cpu.pc() = 0;

	tests::execute_all_machine_cycles<opcodes::and_a_n8>(cpu);
	CHECK_EQ(cpu.reg().a(), 0x01);
}

TEST_CASE("bits.and_a_n8 updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	memory.bus().write(0, 0x01);

	tests::execute_all_machine_cycles<opcodes::and_a_n8>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0xFF;
	cpu.pc() = 0;

	tests::execute_all_machine_cycles<opcodes::and_a_n8>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.xor_a_r8 updates a register properly", test, xor_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xF0;

	test::reg(cpu) = 0x1F;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xEF);
}

TEST_CASE_TEMPLATE("bits.xor_a_r8 does not increment program counter", test, xor_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.xor_a_r8 updates flags properly", test, xor_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xFF;

	test::reg(cpu) = 0xFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::reg(cpu) = 0x0F;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.xor_a_a updates a register properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xF0;
	tests::execute_all_machine_cycles<opcodes::xor_a_a>(cpu);

	CHECK_EQ(cpu.reg().a(), 0x0);
}

TEST_CASE("bits.xor_a_a does not increment program counter")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::xor_a_a>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.xor_a_a updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xFF;
	tests::execute_all_machine_cycles<opcodes::xor_a_a>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0x0F;
	tests::execute_all_machine_cycles<opcodes::xor_a_a>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.xor_a_n8 updates a register properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xF0;
	memory.bus().write(0, 0x1F);

	tests::execute_all_machine_cycles<opcodes::xor_a_n8>(cpu);
	CHECK_EQ(cpu.reg().a(), 0xEF);
}

TEST_CASE("bits.xor_a_n8 updates program counter properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::xor_a_n8>(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("bits.xor_a_n8 updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xFF;
	memory.bus().write(0, 0xFF);

	tests::execute_all_machine_cycles<opcodes::xor_a_n8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	memory.bus().write(0, 0x0F);

	tests::execute_all_machine_cycles<opcodes::xor_a_n8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.xor_a_hl updates a register properly")
{
	const memory::memory_address_t memory_pos = 0xABCD;

	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xF0;
	cpu.reg().hl() = memory_pos;
	memory.bus().write(memory_pos, 0x1F);

	tests::execute_all_machine_cycles<opcodes::xor_a_hl>(cpu);
	CHECK_EQ(cpu.reg().a(), 0xEF);
}

TEST_CASE("bits.xor_a_hl does not increment program counter")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::xor_a_hl>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.xor_a_hl updates flags properly")
{
	const memory::memory_address_t memory_pos = 0xABCD;

	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xFF;
	cpu.reg().hl() = memory_pos;
	memory.bus().write(memory_pos, 0xFF);

	tests::execute_all_machine_cycles<opcodes::xor_a_hl>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	memory.bus().write(memory_pos, 0xFF);
	tests::execute_all_machine_cycles<opcodes::xor_a_hl>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.srl_r8 updates program counter properly", test, srl_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.srl_r8 updates value properly", test, srl_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000100);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000010);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000001);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000000);
}

TEST_CASE_TEMPLATE("bits.srl_r8 updates flags properly", test, srl_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.rr_r8 updates program counter properly", test, rr_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.rr_r8 updates value properly", test, rr_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000100);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000010);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000001);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b10000000);
}

TEST_CASE_TEMPLATE("bits.rr_r8 updates flags properly", test, rr_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.rra updates program counter properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.rra updates value properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0b00001000;

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000100);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000010);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000001);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000000);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b10000000);
}

TEST_CASE("bits.rra updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0b00001000;

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	tests::execute_all_machine_cycles<opcodes::rra>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.or_a_hl updates a register properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xF0;
	cpu.reg().hl() = 0xABCD;
	memory.bus().write(0xABCD, 0x0F);

	tests::execute_all_machine_cycles<opcodes::or_a_hl>(cpu);
	CHECK_EQ(cpu.reg().a(), 0xFF);
}

TEST_CASE("bits.or_a_hl updates program counter properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::or_a_hl>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.or_a_hl updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	cpu.reg().hl() = 0xABCD;

	memory.bus().write(0xABCD, 0x00);
	tests::execute_all_machine_cycles<opcodes::or_a_hl>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	memory.bus().write(0xABCD, 0x0F);
	tests::execute_all_machine_cycles<opcodes::or_a_hl>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.bit_u3_r8 set flags properly", test, bit_u3_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);

	test::reg(cpu) = (0x1 << test::bit_index);
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);

	test::reg(cpu) = test::reg(cpu) ^ 0xFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
}

TEST_CASE_TEMPLATE("bits.bit_u3_r8 updates program counter properly", test, bit_u3_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.res_u3_r8 updates register properly", test, res_u3_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0x0);

	test::reg(cpu) = 0xFF;
	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xFF & ~(1 << test::bit_index));
}

TEST_CASE_TEMPLATE("bits.res_u3_r8 updates program counter properly", test, res_u3_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.set_u3_r8 updates register properly", test, set_u3_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 1 << test::bit_index);

	test::reg(cpu) = 0xFF & ~(1 << test::bit_index);
	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xFF);
}

TEST_CASE_TEMPLATE("bits.set_u3_r8 updates program counter properly", test, set_u3_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.or_a_n8 updates a register properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0xF0;
	memory.bus().write(0, 0x0F);

	tests::execute_all_machine_cycles<opcodes::or_a_n8>(cpu);
	CHECK_EQ(cpu.reg().a(), 0xFF);
}

TEST_CASE("bits.or_a_n8 updates program counter properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::or_a_n8>(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("bits.or_a_n8 updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	memory.bus().write(0, 0x00);
	tests::execute_all_machine_cycles<opcodes::or_a_n8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	memory.bus().write(0, 0x0F);
	tests::execute_all_machine_cycles<opcodes::or_a_n8>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.swap_r8 updates register properly", test, swap_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0xAB;
	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xBA);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xAB);
}

TEST_CASE_TEMPLATE("bits.swap_r8 updates flags properly", test, swap_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().z_flag() = true;
	cpu.reg().n_flag() = true;
	cpu.reg().h_flag() = true;
	cpu.reg().c_flag() = true;

	test::reg(cpu) = 0xAB;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::reg(cpu) = 0x0;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.swap_r8 updates program counter properly", test, swap_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.swap_hl updates register properly")
{
	constexpr memory::memory_address_t memory_pos = 0xABCD;

	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().hl() = memory_pos;
	memory.bus().write(memory_pos, 0xFE);

	tests::execute_all_machine_cycles<opcodes::swap_hl>(cpu);
	CHECK_EQ(memory.bus().read(memory_pos), 0xEF);

	tests::execute_all_machine_cycles<opcodes::swap_hl>(cpu);
	CHECK_EQ(memory.bus().read(memory_pos), 0xFE);
}

TEST_CASE("bits.swap_hl updates flags properly")
{
	constexpr memory::memory_address_t memory_pos = 0xABCD;

	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().z_flag() = true;
	cpu.reg().n_flag() = true;
	cpu.reg().h_flag() = true;
	cpu.reg().c_flag() = true;

	cpu.reg().hl() = memory_pos;
	memory.bus().write(memory_pos, 0xFE);

	tests::execute_all_machine_cycles<opcodes::swap_hl>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	memory.bus().write(memory_pos, 0x0);

	tests::execute_all_machine_cycles<opcodes::swap_hl>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.swap_hl updates program counter properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::swap_hl>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.cpl updates register a properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0;

	tests::execute_all_machine_cycles<opcodes::cpl>(cpu);
	CHECK_EQ(cpu.reg().a(), 0xFF);

	tests::execute_all_machine_cycles<opcodes::cpl>(cpu);
	CHECK_EQ(cpu.reg().a(), 0x0);

	cpu.reg().a() = 0xF0;

	tests::execute_all_machine_cycles<opcodes::cpl>(cpu);
	CHECK_EQ(cpu.reg().a(), 0x0F);
}

TEST_CASE("bits.cpl updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().n_flag() = false;
	cpu.reg().h_flag() = false;
	cpu.reg().z_flag() = false;
	cpu.reg().c_flag() = false;

	tests::execute_all_machine_cycles<opcodes::cpl>(cpu);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::cpl>(cpu);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.cpl updates program counter properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::cpl>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.and_a_r8 updates program counter properly", test, and_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.and_a_r8 updates register a properly", test, and_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	test::reg(cpu) = 0x01;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0);

	cpu.reg().a() = 0xFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0x01);
}

TEST_CASE_TEMPLATE("bits.and_a_r8 updates flags properly", test, and_a_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	test::reg(cpu) = 0x01;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0xFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.and_a_a does not increment program counter")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::and_a_a>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.and_a_a updates register a properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	tests::execute_all_machine_cycles<opcodes::and_a_a>(cpu);

	CHECK_EQ(cpu.reg().a(), 0);

	cpu.reg().a() = 0xFB;
	tests::execute_all_machine_cycles<opcodes::and_a_a>(cpu);

	CHECK_EQ(cpu.reg().a(), 0xFB);
}

TEST_CASE("bits.and_a_a updates flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0x00;
	tests::execute_all_machine_cycles<opcodes::and_a_a>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0xFF;
	tests::execute_all_machine_cycles<opcodes::and_a_a>(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.rlca updates register a properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0b00001000;

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00010000);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00100000);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b01000000);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b10000000);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000001);
}

TEST_CASE("bits.rlca does not increment program counter")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.rlca updates register flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0b00001000;

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rlca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("bits.rla updates register a properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0b00001000;

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00010000);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00100000);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b01000000);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b10000000);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000000);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000001);
}

TEST_CASE("bits.rla does not increment program counter")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.rla updates register flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0b00001000;

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	tests::execute_all_machine_cycles<opcodes::rla>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("bits.rrca updates register a properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0b00001000;

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000100);

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000010);

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000001);

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.reg().a(), 0b10000000);
}

TEST_CASE("bits.rrca does not increment program counter")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("bits.rrca updates register flags properly")
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.reg().a() = 0b00001000;

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	tests::execute_all_machine_cycles<opcodes::rrca>(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE_TEMPLATE("bits.prefix rlc_r8 updates register properly", test, prefix_rlc_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00010000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00100000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b01000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b10000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000001);
}

TEST_CASE_TEMPLATE("bits.prefix rlc_r8 updates program counter properly", test, prefix_rlc_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.prefix rlc_r8 updates register flags properly", test, prefix_rlc_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	test::reg(cpu) = 0x0;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.prefix rrc_r8 updates register a properly", test, prefix_rrc_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000100);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000010);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000001);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b10000000);
}

TEST_CASE_TEMPLATE("bits.prefix rrc_r8 updates program counter properly", test, prefix_rrc_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.prefix rrc_r8 updates register flags properly", test, prefix_rrc_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	test::reg(cpu) = 0x0;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.prefix rl_r8 updates register properly", test, prefix_rl_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00010000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00100000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b01000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b10000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000001);
}

TEST_CASE_TEMPLATE("bits.prefix rl_r8 updates program counter properly", test, prefix_rl_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.prefix rl_r8 updates register flags properly", test, prefix_rl_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.sla_r8 updates register properly", test, sla_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00010000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00100000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b01000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b10000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b00000000);
}

TEST_CASE_TEMPLATE("bits.sla_r8 updates program counter properly", test, sla_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.sla_r8 updates register flags properly", test, sla_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bits.sra_r8 updates register properly", test, sra_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b10001000;

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b11000100);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b11100010);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b11110001);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b11111000);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b11111100);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b11111110);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b11111111);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0b11111111);
}

TEST_CASE_TEMPLATE("bits.sra_r8 updates program counter properly", test, sra_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("bits.sra_r8 updates register flags properly", test, sra_r8_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	test::reg(cpu) = 0b00001000;

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}
