
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
