
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

	auto get_a(cpu::cpu& cpu) { return cpu.reg().a(); }
	auto get_b(cpu::cpu& cpu) { return cpu.reg().b(); }
	auto get_c(cpu::cpu& cpu) { return cpu.reg().c(); }
	auto get_d(cpu::cpu& cpu) { return cpu.reg().d(); }
	auto get_e(cpu::cpu& cpu) { return cpu.reg().e(); }
	auto get_h(cpu::cpu& cpu) { return cpu.reg().h(); }
	auto get_l(cpu::cpu& cpu) { return cpu.reg().l(); }

	template<opcodes::Instruction OpCode, auto RegFn>
	requires R8RegisterFetchFn<RegFn>
	struct r8_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_8 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	template<opcodes::Instruction OpCode, auto RegFn, std::uint8_t BitIndex>
	requires R8RegisterFetchFn<RegFn> && (sizeof(BitIndex) <= 7)
	struct r8_u3_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static constexpr auto bit_index = BitIndex;

		static cpu::register_8 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	#define or_a_r8_test_cases \
    r8_test_case<opcodes::or_a_b, get_b>, \
    r8_test_case<opcodes::or_a_c, get_c>, \
    r8_test_case<opcodes::or_a_d, get_d>, \
    r8_test_case<opcodes::or_a_e, get_e>, \
    r8_test_case<opcodes::or_a_h, get_h>, \
    r8_test_case<opcodes::or_a_l, get_l>

	#define xor_a_r8_test_cases \
    r8_test_case<opcodes::xor_a_b, get_b>, \
    r8_test_case<opcodes::xor_a_c, get_c>, \
    r8_test_case<opcodes::xor_a_d, get_d>, \
    r8_test_case<opcodes::xor_a_e, get_e>, \
    r8_test_case<opcodes::xor_a_h, get_h>, \
    r8_test_case<opcodes::xor_a_l, get_l>

	#define srl_r8_test_cases \
    r8_test_case<opcodes::srl_a, get_a>, \
    r8_test_case<opcodes::srl_b, get_b>, \
    r8_test_case<opcodes::srl_c, get_c>, \
    r8_test_case<opcodes::srl_d, get_d>, \
    r8_test_case<opcodes::srl_e, get_e>, \
    r8_test_case<opcodes::srl_h, get_h>, \
    r8_test_case<opcodes::srl_l, get_l>

	#define rr_r8_test_cases \
    r8_test_case<opcodes::rr_a, get_a>, \
    r8_test_case<opcodes::rr_b, get_b>, \
    r8_test_case<opcodes::rr_c, get_c>, \
    r8_test_case<opcodes::rr_d, get_d>, \
    r8_test_case<opcodes::rr_e, get_e>, \
    r8_test_case<opcodes::rr_h, get_h>, \
    r8_test_case<opcodes::rr_l, get_l>

	#define bit_u3_r8_test_cases \
    r8_u3_test_case<opcodes::bit_0_a, get_a, 0>, \
    r8_u3_test_case<opcodes::bit_1_a, get_a, 1>, \
    r8_u3_test_case<opcodes::bit_2_a, get_a, 2>, \
    r8_u3_test_case<opcodes::bit_3_a, get_a, 3>, \
    r8_u3_test_case<opcodes::bit_4_a, get_a, 4>, \
    r8_u3_test_case<opcodes::bit_5_a, get_a, 5>, \
    r8_u3_test_case<opcodes::bit_6_a, get_a, 6>, \
    r8_u3_test_case<opcodes::bit_7_a, get_a, 7>, \
    r8_u3_test_case<opcodes::bit_0_b, get_b, 0>, \
    r8_u3_test_case<opcodes::bit_1_b, get_b, 1>, \
    r8_u3_test_case<opcodes::bit_2_b, get_b, 2>, \
    r8_u3_test_case<opcodes::bit_3_b, get_b, 3>, \
    r8_u3_test_case<opcodes::bit_4_b, get_b, 4>, \
    r8_u3_test_case<opcodes::bit_5_b, get_b, 5>, \
    r8_u3_test_case<opcodes::bit_6_b, get_b, 6>, \
    r8_u3_test_case<opcodes::bit_7_b, get_b, 7>, \
    r8_u3_test_case<opcodes::bit_0_c, get_c, 0>, \
    r8_u3_test_case<opcodes::bit_1_c, get_c, 1>, \
    r8_u3_test_case<opcodes::bit_2_c, get_c, 2>, \
    r8_u3_test_case<opcodes::bit_3_c, get_c, 3>, \
    r8_u3_test_case<opcodes::bit_4_c, get_c, 4>, \
    r8_u3_test_case<opcodes::bit_5_c, get_c, 5>, \
    r8_u3_test_case<opcodes::bit_6_c, get_c, 6>, \
    r8_u3_test_case<opcodes::bit_7_c, get_c, 7>, \
    r8_u3_test_case<opcodes::bit_0_d, get_d, 0>, \
    r8_u3_test_case<opcodes::bit_1_d, get_d, 1>, \
    r8_u3_test_case<opcodes::bit_2_d, get_d, 2>, \
    r8_u3_test_case<opcodes::bit_3_d, get_d, 3>, \
    r8_u3_test_case<opcodes::bit_4_d, get_d, 4>, \
    r8_u3_test_case<opcodes::bit_5_d, get_d, 5>, \
    r8_u3_test_case<opcodes::bit_6_d, get_d, 6>, \
    r8_u3_test_case<opcodes::bit_7_d, get_d, 7>, \
    r8_u3_test_case<opcodes::bit_0_e, get_e, 0>, \
    r8_u3_test_case<opcodes::bit_1_e, get_e, 1>, \
    r8_u3_test_case<opcodes::bit_2_e, get_e, 2>, \
    r8_u3_test_case<opcodes::bit_3_e, get_e, 3>, \
    r8_u3_test_case<opcodes::bit_4_e, get_e, 4>, \
    r8_u3_test_case<opcodes::bit_5_e, get_e, 5>, \
    r8_u3_test_case<opcodes::bit_6_e, get_e, 6>, \
    r8_u3_test_case<opcodes::bit_7_e, get_e, 7>, \
    r8_u3_test_case<opcodes::bit_0_h, get_h, 0>, \
    r8_u3_test_case<opcodes::bit_1_h, get_h, 1>, \
    r8_u3_test_case<opcodes::bit_2_h, get_h, 2>, \
    r8_u3_test_case<opcodes::bit_3_h, get_h, 3>, \
    r8_u3_test_case<opcodes::bit_4_h, get_h, 4>, \
    r8_u3_test_case<opcodes::bit_5_h, get_h, 5>, \
    r8_u3_test_case<opcodes::bit_6_h, get_h, 6>, \
    r8_u3_test_case<opcodes::bit_7_h, get_h, 7>, \
    r8_u3_test_case<opcodes::bit_0_l, get_l, 0>, \
    r8_u3_test_case<opcodes::bit_1_l, get_l, 1>, \
    r8_u3_test_case<opcodes::bit_2_l, get_l, 2>, \
    r8_u3_test_case<opcodes::bit_3_l, get_l, 3>, \
    r8_u3_test_case<opcodes::bit_4_l, get_l, 4>, \
    r8_u3_test_case<opcodes::bit_5_l, get_l, 5>, \
    r8_u3_test_case<opcodes::bit_6_l, get_l, 6>, \
    r8_u3_test_case<opcodes::bit_7_l, get_l, 7>

	#define res_u3_r8_test_cases \
    r8_u3_test_case<opcodes::res_0_a, get_a, 0>, \
    r8_u3_test_case<opcodes::res_1_a, get_a, 1>, \
    r8_u3_test_case<opcodes::res_2_a, get_a, 2>, \
    r8_u3_test_case<opcodes::res_3_a, get_a, 3>, \
    r8_u3_test_case<opcodes::res_4_a, get_a, 4>, \
    r8_u3_test_case<opcodes::res_5_a, get_a, 5>, \
    r8_u3_test_case<opcodes::res_6_a, get_a, 6>, \
    r8_u3_test_case<opcodes::res_7_a, get_a, 7>, \
    r8_u3_test_case<opcodes::res_0_b, get_b, 0>, \
    r8_u3_test_case<opcodes::res_1_b, get_b, 1>, \
    r8_u3_test_case<opcodes::res_2_b, get_b, 2>, \
    r8_u3_test_case<opcodes::res_3_b, get_b, 3>, \
    r8_u3_test_case<opcodes::res_4_b, get_b, 4>, \
    r8_u3_test_case<opcodes::res_5_b, get_b, 5>, \
    r8_u3_test_case<opcodes::res_6_b, get_b, 6>, \
    r8_u3_test_case<opcodes::res_7_b, get_b, 7>, \
    r8_u3_test_case<opcodes::res_0_c, get_c, 0>, \
    r8_u3_test_case<opcodes::res_1_c, get_c, 1>, \
    r8_u3_test_case<opcodes::res_2_c, get_c, 2>, \
    r8_u3_test_case<opcodes::res_3_c, get_c, 3>, \
    r8_u3_test_case<opcodes::res_4_c, get_c, 4>, \
    r8_u3_test_case<opcodes::res_5_c, get_c, 5>, \
    r8_u3_test_case<opcodes::res_6_c, get_c, 6>, \
    r8_u3_test_case<opcodes::res_7_c, get_c, 7>, \
    r8_u3_test_case<opcodes::res_0_d, get_d, 0>, \
    r8_u3_test_case<opcodes::res_1_d, get_d, 1>, \
    r8_u3_test_case<opcodes::res_2_d, get_d, 2>, \
    r8_u3_test_case<opcodes::res_3_d, get_d, 3>, \
    r8_u3_test_case<opcodes::res_4_d, get_d, 4>, \
    r8_u3_test_case<opcodes::res_5_d, get_d, 5>, \
    r8_u3_test_case<opcodes::res_6_d, get_d, 6>, \
    r8_u3_test_case<opcodes::res_7_d, get_d, 7>, \
    r8_u3_test_case<opcodes::res_0_e, get_e, 0>, \
    r8_u3_test_case<opcodes::res_1_e, get_e, 1>, \
    r8_u3_test_case<opcodes::res_2_e, get_e, 2>, \
    r8_u3_test_case<opcodes::res_3_e, get_e, 3>, \
    r8_u3_test_case<opcodes::res_4_e, get_e, 4>, \
    r8_u3_test_case<opcodes::res_5_e, get_e, 5>, \
    r8_u3_test_case<opcodes::res_6_e, get_e, 6>, \
    r8_u3_test_case<opcodes::res_7_e, get_e, 7>, \
    r8_u3_test_case<opcodes::res_0_h, get_h, 0>, \
    r8_u3_test_case<opcodes::res_1_h, get_h, 1>, \
    r8_u3_test_case<opcodes::res_2_h, get_h, 2>, \
    r8_u3_test_case<opcodes::res_3_h, get_h, 3>, \
    r8_u3_test_case<opcodes::res_4_h, get_h, 4>, \
    r8_u3_test_case<opcodes::res_5_h, get_h, 5>, \
    r8_u3_test_case<opcodes::res_6_h, get_h, 6>, \
    r8_u3_test_case<opcodes::res_7_h, get_h, 7>, \
    r8_u3_test_case<opcodes::res_0_l, get_l, 0>, \
    r8_u3_test_case<opcodes::res_1_l, get_l, 1>, \
    r8_u3_test_case<opcodes::res_2_l, get_l, 2>, \
    r8_u3_test_case<opcodes::res_3_l, get_l, 3>, \
    r8_u3_test_case<opcodes::res_4_l, get_l, 4>, \
    r8_u3_test_case<opcodes::res_5_l, get_l, 5>, \
    r8_u3_test_case<opcodes::res_6_l, get_l, 6>, \
    r8_u3_test_case<opcodes::res_7_l, get_l, 7>

	#define set_u3_r8_test_cases \
    r8_u3_test_case<opcodes::set_0_a, get_a, 0>, \
    r8_u3_test_case<opcodes::set_1_a, get_a, 1>, \
    r8_u3_test_case<opcodes::set_2_a, get_a, 2>, \
    r8_u3_test_case<opcodes::set_3_a, get_a, 3>, \
    r8_u3_test_case<opcodes::set_4_a, get_a, 4>, \
    r8_u3_test_case<opcodes::set_5_a, get_a, 5>, \
    r8_u3_test_case<opcodes::set_6_a, get_a, 6>, \
    r8_u3_test_case<opcodes::set_7_a, get_a, 7>, \
    r8_u3_test_case<opcodes::set_0_b, get_b, 0>, \
    r8_u3_test_case<opcodes::set_1_b, get_b, 1>, \
    r8_u3_test_case<opcodes::set_2_b, get_b, 2>, \
    r8_u3_test_case<opcodes::set_3_b, get_b, 3>, \
    r8_u3_test_case<opcodes::set_4_b, get_b, 4>, \
    r8_u3_test_case<opcodes::set_5_b, get_b, 5>, \
    r8_u3_test_case<opcodes::set_6_b, get_b, 6>, \
    r8_u3_test_case<opcodes::set_7_b, get_b, 7>, \
    r8_u3_test_case<opcodes::set_0_c, get_c, 0>, \
    r8_u3_test_case<opcodes::set_1_c, get_c, 1>, \
    r8_u3_test_case<opcodes::set_2_c, get_c, 2>, \
    r8_u3_test_case<opcodes::set_3_c, get_c, 3>, \
    r8_u3_test_case<opcodes::set_4_c, get_c, 4>, \
    r8_u3_test_case<opcodes::set_5_c, get_c, 5>, \
    r8_u3_test_case<opcodes::set_6_c, get_c, 6>, \
    r8_u3_test_case<opcodes::set_7_c, get_c, 7>, \
    r8_u3_test_case<opcodes::set_0_d, get_d, 0>, \
    r8_u3_test_case<opcodes::set_1_d, get_d, 1>, \
    r8_u3_test_case<opcodes::set_2_d, get_d, 2>, \
    r8_u3_test_case<opcodes::set_3_d, get_d, 3>, \
    r8_u3_test_case<opcodes::set_4_d, get_d, 4>, \
    r8_u3_test_case<opcodes::set_5_d, get_d, 5>, \
    r8_u3_test_case<opcodes::set_6_d, get_d, 6>, \
    r8_u3_test_case<opcodes::set_7_d, get_d, 7>, \
    r8_u3_test_case<opcodes::set_0_e, get_e, 0>, \
    r8_u3_test_case<opcodes::set_1_e, get_e, 1>, \
    r8_u3_test_case<opcodes::set_2_e, get_e, 2>, \
    r8_u3_test_case<opcodes::set_3_e, get_e, 3>, \
    r8_u3_test_case<opcodes::set_4_e, get_e, 4>, \
    r8_u3_test_case<opcodes::set_5_e, get_e, 5>, \
    r8_u3_test_case<opcodes::set_6_e, get_e, 6>, \
    r8_u3_test_case<opcodes::set_7_e, get_e, 7>, \
    r8_u3_test_case<opcodes::set_0_h, get_h, 0>, \
    r8_u3_test_case<opcodes::set_1_h, get_h, 1>, \
    r8_u3_test_case<opcodes::set_2_h, get_h, 2>, \
    r8_u3_test_case<opcodes::set_3_h, get_h, 3>, \
    r8_u3_test_case<opcodes::set_4_h, get_h, 4>, \
    r8_u3_test_case<opcodes::set_5_h, get_h, 5>, \
    r8_u3_test_case<opcodes::set_6_h, get_h, 6>, \
    r8_u3_test_case<opcodes::set_7_h, get_h, 7>, \
    r8_u3_test_case<opcodes::set_0_l, get_l, 0>, \
    r8_u3_test_case<opcodes::set_1_l, get_l, 1>, \
    r8_u3_test_case<opcodes::set_2_l, get_l, 2>, \
    r8_u3_test_case<opcodes::set_3_l, get_l, 3>, \
    r8_u3_test_case<opcodes::set_4_l, get_l, 4>, \
    r8_u3_test_case<opcodes::set_5_l, get_l, 5>, \
    r8_u3_test_case<opcodes::set_6_l, get_l, 6>, \
    r8_u3_test_case<opcodes::set_7_l, get_l, 7>

	#define swap_r8_test_cases \
    r8_test_case<opcodes::swap_a, get_a>, \
    r8_test_case<opcodes::swap_b, get_b>, \
    r8_test_case<opcodes::swap_c, get_c>, \
    r8_test_case<opcodes::swap_d, get_d>, \
    r8_test_case<opcodes::swap_e, get_e>, \
    r8_test_case<opcodes::swap_h, get_h>, \
    r8_test_case<opcodes::swap_l, get_l>

	#define and_a_r8_test_cases \
    r8_test_case<opcodes::and_a_b, get_b>, \
    r8_test_case<opcodes::and_a_c, get_c>, \
    r8_test_case<opcodes::and_a_d, get_d>, \
    r8_test_case<opcodes::and_a_e, get_e>, \
    r8_test_case<opcodes::and_a_h, get_h>, \
    r8_test_case<opcodes::and_a_l, get_l>

	#define prefix_rlc_r8_test_cases \
    r8_test_case<opcodes::prefix_rlc_a, get_a>, \
    r8_test_case<opcodes::prefix_rlc_b, get_b>, \
    r8_test_case<opcodes::prefix_rlc_c, get_c>, \
    r8_test_case<opcodes::prefix_rlc_d, get_d>, \
    r8_test_case<opcodes::prefix_rlc_e, get_e>, \
    r8_test_case<opcodes::prefix_rlc_h, get_h>, \
    r8_test_case<opcodes::prefix_rlc_l, get_l>

	#define prefix_rrc_r8_test_cases \
    r8_test_case<opcodes::prefix_rrc_a, get_a>, \
    r8_test_case<opcodes::prefix_rrc_b, get_b>, \
    r8_test_case<opcodes::prefix_rrc_c, get_c>, \
    r8_test_case<opcodes::prefix_rrc_d, get_d>, \
    r8_test_case<opcodes::prefix_rrc_e, get_e>, \
    r8_test_case<opcodes::prefix_rrc_h, get_h>, \
    r8_test_case<opcodes::prefix_rrc_l, get_l>

	#define prefix_rl_r8_test_cases \
    r8_test_case<opcodes::prefix_rl_a, get_a>, \
    r8_test_case<opcodes::prefix_rl_b, get_b>, \
    r8_test_case<opcodes::prefix_rl_c, get_c>, \
    r8_test_case<opcodes::prefix_rl_d, get_d>, \
    r8_test_case<opcodes::prefix_rl_e, get_e>, \
    r8_test_case<opcodes::prefix_rl_h, get_h>, \
    r8_test_case<opcodes::prefix_rl_l, get_l>

	#define sla_r8_test_cases \
    r8_test_case<opcodes::sla_a, get_a>, \
    r8_test_case<opcodes::sla_b, get_b>, \
    r8_test_case<opcodes::sla_c, get_c>, \
    r8_test_case<opcodes::sla_d, get_d>, \
    r8_test_case<opcodes::sla_e, get_e>, \
    r8_test_case<opcodes::sla_h, get_h>, \
    r8_test_case<opcodes::sla_l, get_l>

	#define sra_r8_test_cases \
    r8_test_case<opcodes::sra_a, get_a>, \
    r8_test_case<opcodes::sra_b, get_b>, \
    r8_test_case<opcodes::sra_c, get_c>, \
    r8_test_case<opcodes::sra_d, get_d>, \
    r8_test_case<opcodes::sra_e, get_e>, \
    r8_test_case<opcodes::sra_h, get_h>, \
    r8_test_case<opcodes::sra_l, get_l>
}

TEST_CASE_TEMPLATE("or_a_r8 updates a register properly", test, or_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xF0;

	test::reg(cpu) = 0x0F;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xFF);
}

TEST_CASE_TEMPLATE("or_a_r8 updates program counter properly", test, or_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("or_a_r8 updates flags properly", test, or_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE("or_a_a updates a register properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xF0;
	opcodes::or_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xF0);
}

TEST_CASE("or_a_a updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::or_a_a::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("or_a_a updates flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x00;
	opcodes::or_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0x0F;
	opcodes::or_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("and_a_n8 updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::and_a_n8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("and_a_n8 updates register a properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x00;
	memory[1] = 0x01;

	opcodes::and_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0);

	cpu.reg().a() = 0xFF;
	cpu.pc() = 0;

	opcodes::and_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0x01);
}

TEST_CASE("and_a_n8 updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x00;
	memory[1] = 0x01;

	opcodes::and_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0xFF;
	cpu.pc() = 0;

	opcodes::and_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("xor_a_r8 updates a register properly", test, xor_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xF0;

	test::reg(cpu) = 0x1F;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xEF);
}

TEST_CASE_TEMPLATE("xor_a_r8 updates program counter properly", test, xor_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("xor_a_r8 updates flags properly", test, xor_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE("xor_a_a updates a register properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xF0;
	opcodes::xor_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0x0);
}

TEST_CASE("xor_a_a updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::xor_a_a::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("xor_a_a updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xFF;
	opcodes::xor_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0x0F;
	opcodes::xor_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("xor_a_n8 updates a register properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xF0;
	memory[1] = 0x1F;

	opcodes::xor_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xEF);
}

TEST_CASE("xor_a_n8 updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::xor_a_n8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("xor_a_n8 updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xFF;
	memory[1] = 0xFF;

	opcodes::xor_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	memory[1] = 0x0F;
	opcodes::xor_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("xor_a_hl updates a register properly")
{
	const cpu::memory_bus::index_t memory_pos = 0xABCD;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xF0;
	cpu.reg().hl() = memory_pos;
	memory[memory_pos] = 0x1F;

	opcodes::xor_a_hl::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0xEF);
}

TEST_CASE("xor_a_hl updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::xor_a_hl::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("xor_a_hl updates flags properly")
{
	const cpu::memory_bus::index_t memory_pos = 0xABCD;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xFF;
	cpu.reg().hl() = memory_pos;
	memory[memory_pos] = 0xFF;

	opcodes::xor_a_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	memory[memory_pos] = 0xFF;
	opcodes::xor_a_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("srl_r8 updates program counter properly", test, srl_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("srl_r8 updates value properly", test, srl_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("srl_r8 updates flags properly", test, srl_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("rr_r8 updates program counter properly", test, rr_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("rr_r8 updates value properly", test, rr_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("rr_r8 updates flags properly", test, rr_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE("rra updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("rra updates value properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0b00001000;

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000100);

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000010);

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000001);

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000000);

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b10000000);
}

TEST_CASE("rra updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0b00001000;

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	opcodes::rra::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("or_a_hl updates a register properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xF0;
	cpu.reg().hl() = 0xABCD;
	memory[0xABCD] = 0x0F;

	opcodes::or_a_hl::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xFF);
}

TEST_CASE("or_a_hl updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::or_a_hl::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("or_a_hl updates flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x00;
	cpu.reg().hl() = 0xABCD;

	memory[0xABCD] = 0x00;
	opcodes::or_a_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	memory[0xABCD] = 0x0F;
	opcodes::or_a_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("bit_u3_r8 set flags properly", test, bit_u3_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("bit_u3_r8 updates program counter properly", test, bit_u3_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("res_u3_r8 updates register properly", test, res_u3_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0x0);

	test::reg(cpu) = 0xFF;
	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xFF & ~(1 << test::bit_index));
}

TEST_CASE_TEMPLATE("res_u3_r8 updates program counter properly", test, res_u3_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("set_u3_r8 updates register properly", test, set_u3_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 1 << test::bit_index);

	test::reg(cpu) = 0xFF & ~(1 << test::bit_index);
	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xFF);
}

TEST_CASE_TEMPLATE("set_u3_r8 updates program counter properly", test, set_u3_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("or_a_n8 updates a register properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xF0;
	memory[1] = 0x0F;

	opcodes::or_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xFF);
}

TEST_CASE("or_a_n8 updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::or_a_n8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("or_a_n8 updates flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x00;
	memory[1] = 0x00;

	opcodes::or_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	memory[1] = 0x0F;
	opcodes::or_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("swap_r8 updates register properly", test, swap_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::reg(cpu) = 0xAB;
	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xBA);

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 0xAB);
}

TEST_CASE_TEMPLATE("swap_r8 updates flags properly", test, swap_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("swap_r8 updates program counter properly", test, swap_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("swap_hl updates register properly")
{
	constexpr cpu::memory_bus::index_t memory_pos = 0xABCD;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().hl() = memory_pos;
	memory[memory_pos] = 0xFE;

	opcodes::swap_hl::execute(cpu);
	CHECK_EQ(memory[memory_pos], 0xEF);

	opcodes::swap_hl::execute(cpu);
	CHECK_EQ(memory[memory_pos], 0xFE);
}

TEST_CASE("swap_hl updates flags properly")
{
	constexpr cpu::memory_bus::index_t memory_pos = 0xABCD;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().z_flag() = true;
	cpu.reg().n_flag() = true;
	cpu.reg().h_flag() = true;
	cpu.reg().c_flag() = true;

	cpu.reg().hl() = memory_pos;
	memory[memory_pos] = 0xFE;

	opcodes::swap_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	memory[memory_pos] = 0x0;

	opcodes::swap_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("swap_hl updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::swap_hl::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("cpl updates register a properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0;

	opcodes::cpl::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0xFF);

	opcodes::cpl::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0x0);

	cpu.reg().a() = 0xF0;

	opcodes::cpl::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0x0F);
}

TEST_CASE("cpl updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().n_flag() = false;
	cpu.reg().h_flag() = false;
	cpu.reg().z_flag() = false;
	cpu.reg().c_flag() = false;

	opcodes::cpl::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::cpl::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("cpl updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::cpl::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("and_a_r8 updates program counter properly", test, and_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("and_a_r8 updates register a properly", test, and_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x00;
	test::reg(cpu) = 0x01;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0);

	cpu.reg().a() = 0xFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0x01);
}

TEST_CASE_TEMPLATE("and_a_r8 updates flags properly", test, and_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE("and_a_a updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::and_a_a::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("and_a_a updates register a properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x00;
	opcodes::and_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0);

	cpu.reg().a() = 0xFB;
	opcodes::and_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xFB);
}

TEST_CASE("and_a_a updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x00;
	opcodes::and_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0xFF;
	opcodes::and_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("rlca updates register a properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0b00001000;

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00010000);

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00100000);

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b01000000);

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b10000000);

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000001);
}

TEST_CASE("rlca updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("rlca updates register flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0b00001000;

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rlca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("rla updates register a properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0b00001000;

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00010000);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00100000);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b01000000);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b10000000);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000000);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000001);
}

TEST_CASE("rla updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("rla updates register flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0b00001000;

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	opcodes::rla::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("rrca updates register a properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0b00001000;

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000100);

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000010);

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b00000001);

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0b10000000);
}

TEST_CASE("rrca updates program counter properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("rrca updates register flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0b00001000;

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	opcodes::rrca::execute(cpu);
	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE_TEMPLATE("prefix rlc_r8 updates register properly", test, prefix_rlc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("prefix rlc_r8 updates program counter properly", test, prefix_rlc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("prefix rlc_r8 updates register flags properly", test, prefix_rlc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("prefix rrc_r8 updates register a properly", test, prefix_rrc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("prefix rrc_r8 updates program counter properly", test, prefix_rrc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("prefix rrc_r8 updates register flags properly", test, prefix_rrc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("prefix rl_r8 updates register properly", test, prefix_rl_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("prefix rl_r8 updates program counter properly", test, prefix_rl_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("prefix rl_r8 updates register flags properly", test, prefix_rl_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("sla_r8 updates register properly", test, sla_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("sla_r8 updates program counter properly", test, sla_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("sla_r8 updates register flags properly", test, sla_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("sra_r8 updates register properly", test, sra_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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

TEST_CASE_TEMPLATE("sra_r8 updates program counter properly", test, sra_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("sra_r8 updates register flags properly", test, sra_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

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
