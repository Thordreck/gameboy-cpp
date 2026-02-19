
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

    template <auto RegFn>
    concept R16RegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_16>;
    };

	auto get_af(cpu::cpu& cpu) { return cpu.reg().af(); }
	auto get_bc(cpu::cpu& cpu) { return cpu.reg().bc(); }
	auto get_de(cpu::cpu& cpu) { return cpu.reg().de(); }
	auto get_hl(cpu::cpu& cpu) { return cpu.reg().hl(); }

	template<opcodes::Instruction OpCode, auto RegFn>
	requires R16RegisterFetchFn<RegFn>
	struct r16_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_16 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	#define add_a_r8_test_cases \
    r8_test_case<opcodes::add_a_b, get_b>, \
    r8_test_case<opcodes::add_a_c, get_c>, \
    r8_test_case<opcodes::add_a_d, get_d>, \
    r8_test_case<opcodes::add_a_e, get_e>, \
    r8_test_case<opcodes::add_a_h, get_h>, \
    r8_test_case<opcodes::add_a_l, get_l>

	#define inc_r8_test_cases \
    r8_test_case<opcodes::inc_a, get_a>, \
    r8_test_case<opcodes::inc_b, get_b>, \
    r8_test_case<opcodes::inc_c, get_c>, \
    r8_test_case<opcodes::inc_d, get_d>, \
    r8_test_case<opcodes::inc_e, get_e>, \
    r8_test_case<opcodes::inc_h, get_h>, \
    r8_test_case<opcodes::inc_l, get_l>

	#define dec_r8_test_cases \
    r8_test_case<opcodes::dec_a, get_a>, \
    r8_test_case<opcodes::dec_b, get_b>, \
    r8_test_case<opcodes::dec_c, get_c>, \
    r8_test_case<opcodes::dec_d, get_d>, \
    r8_test_case<opcodes::dec_e, get_e>, \
    r8_test_case<opcodes::dec_h, get_h>, \
    r8_test_case<opcodes::dec_l, get_l>

	#define inc_r16_test_cases \
    r16_test_case<opcodes::inc_bc, get_bc>, \
    r16_test_case<opcodes::inc_de, get_de>, \
    r16_test_case<opcodes::inc_hl, get_hl>

	#define cp_a_r8_test_cases \
    r8_test_case<opcodes::cp_a_b, get_b>, \
    r8_test_case<opcodes::cp_a_c, get_c>, \
    r8_test_case<opcodes::cp_a_d, get_d>, \
    r8_test_case<opcodes::cp_a_e, get_e>, \
    r8_test_case<opcodes::cp_a_h, get_h>, \
    r8_test_case<opcodes::cp_a_l, get_l>

	#define adc_a_r8_test_cases \
    r8_test_case<opcodes::adc_a_b, get_b>, \
    r8_test_case<opcodes::adc_a_c, get_c>, \
    r8_test_case<opcodes::adc_a_d, get_d>, \
    r8_test_case<opcodes::adc_a_e, get_e>, \
    r8_test_case<opcodes::adc_a_h, get_h>, \
    r8_test_case<opcodes::adc_a_l, get_l>

	#define add_hl_r16_test_cases \
    r16_test_case<opcodes::add_hl_bc, get_bc>, \
    r16_test_case<opcodes::add_hl_de, get_de>
}

TEST_CASE_TEMPLATE("add_a_r8 updates a register properly", test, add_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0);

	test::reg(cpu) = 5;
	test::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 5);

	test::reg(cpu) = 125;
	test::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 130);

	test::reg(cpu) = 127;
	test::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 1);
}

TEST_CASE_TEMPLATE("add_a_r8 updates zero flag properly", test, add_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK(cpu.reg().z_flag());

	test::reg(cpu) = 5;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg().z_flag());

	test::reg(cpu) = 251;
	test::execute(cpu);
	CHECK(cpu.reg().z_flag());
}

TEST_CASE_TEMPLATE("add_a_r8 always set substraction flag to zero", test, add_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_FALSE(cpu.reg().n_flag());

	test::reg(cpu) = 5;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg().n_flag());

	test::reg(cpu) = 252;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg().n_flag());
}

TEST_CASE_TEMPLATE("add_a_r8 applies carry flag properly", test, add_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_FALSE(cpu.reg().c_flag());

	test::reg(cpu) = 255;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg().c_flag());

	test::reg(cpu) = 1;
	test::execute(cpu);
	CHECK(cpu.reg().c_flag());

	test::execute(cpu);
	CHECK_FALSE(cpu.reg().c_flag());
}

TEST_CASE_TEMPLATE("add_a_r8 applies half carry flag properly", test, add_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_FALSE(cpu.reg().h_flag());

	test::reg(cpu) = 0xF;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg().h_flag());

	test::execute(cpu);
	CHECK(cpu.reg().h_flag());

	test::reg(cpu) = 0;
	test::execute(cpu);
	CHECK_FALSE(cpu.reg().h_flag());
}

TEST_CASE_TEMPLATE("add_a_r8 increments program counter properly", test, add_a_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	const cpu::program_counter previous_pc = cpu.pc();
	test::execute(cpu);
	CHECK_EQ(previous_pc + 1, cpu.pc());
}

TEST_CASE_TEMPLATE("inc_r8 increments value", test, inc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 1);
}

TEST_CASE_TEMPLATE("inc_r8 increments program counter", test, inc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("inc_r8 sets flags properly", test, inc_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);

	test::reg(cpu) = 0xFE;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
}

TEST_CASE_TEMPLATE("dec_r8 decrements value", test, dec_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::reg(cpu) = 0xFF;
	test::execute(cpu);

	CHECK_EQ(test::reg(cpu), 0xFE);
}

TEST_CASE_TEMPLATE("dec_r8 increments program counter", test, dec_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("dec_r8 sets flags properly", test, dec_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), true);

	test::reg(cpu) = 0x2;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
}

TEST_CASE_TEMPLATE("inc_r16 increments value", test, inc_r16_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(test::reg(cpu), 1);
}

TEST_CASE_TEMPLATE("inc_r16 increments program counter", test, inc_r16_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("inc_r16 does not modify any flags", test, inc_r16_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);

	test::reg(cpu) = 0xFFFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
}

TEST_CASE("cp_a_n8 updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xBB;
	memory[1] = 0xFF;

	opcodes::cp_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), true);

	cpu.pc() = 0;
	cpu.reg().a() = 0xFF;

	opcodes::cp_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("cp_a_n8 does not modify register a")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	memory[1] = 0xFF;
	opcodes::cp_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0);
}

TEST_CASE("cp_a_n8 increments pc properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::cp_a_n8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("cp_a_r8 updates flags properly", test, cp_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xBB;
	test::reg(cpu) = 0xFF;

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), true);

	cpu.reg().a() = 0xFF;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE_TEMPLATE("cp_a_r8 does not modify register a", test, cp_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::reg(cpu) = 0xFF;
	const cpu::register_8::type_t original_value = cpu.reg().a();

	test::execute(cpu);
	CHECK_EQ(cpu.reg().a(), original_value);
}

TEST_CASE_TEMPLATE("cp_a_r8 increments pc properly", test, cp_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("cp_a_a updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xBB;
	opcodes::cp_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0xFF;
	opcodes::cp_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("cp_a_a does not modify register a")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xFF;
	opcodes::cp_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xFF);
}

TEST_CASE("cp_a_a increments pc properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::cp_a_a::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("add_a_n8 updates a register properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0);

	cpu.pc() = 0;
	memory[1] = 5;
	opcodes::add_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 5);

	cpu.pc() = 0;
	memory[1] = 125;
	opcodes::add_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 130);

	cpu.pc() = 0;
	memory[1] = 127;
	opcodes::add_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 1);
}

TEST_CASE("add_a_n8 updates zero flag properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_a_n8::execute(cpu);
	CHECK(cpu.reg().z_flag());

	cpu.pc() = 0;
	memory[1] = 5;
	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().z_flag());

	cpu.pc() = 0;
	memory[1] = 251;
	opcodes::add_a_n8::execute(cpu);
	CHECK(cpu.reg().z_flag());
}

TEST_CASE("add_a_n8 always set substraction flag to zero")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().n_flag());

	cpu.pc() = 0;
	memory[1] = 5;
	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().n_flag());

	cpu.pc() = 0;
	memory[1] = 252;
	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().n_flag());
}

TEST_CASE("add_a_n8 applies carry flag properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().c_flag());

	cpu.pc() = 0;
	memory[1] = 255;
	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().c_flag());

	cpu.pc() = 0;
	memory[1] = 1;
	opcodes::add_a_n8::execute(cpu);
	CHECK(cpu.reg().c_flag());

	cpu.pc() = 0;
	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().c_flag());
}

TEST_CASE("add_a_n8 applies half carry flag properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().h_flag());

	cpu.pc() = 0;
	memory[1] = 0xF;
	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().h_flag());

	cpu.pc() = 0;
	opcodes::add_a_n8::execute(cpu);
	CHECK(cpu.reg().h_flag());

	cpu.pc() = 0;
	memory[1] = 0;
	opcodes::add_a_n8::execute(cpu);
	CHECK_FALSE(cpu.reg().h_flag());
}

TEST_CASE("add_a_n8 increments program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_a_n8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("sub_a_n8 substracts value properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0xFF;
	memory[1] = 0xF0;

	opcodes::sub_a_n8::execute(cpu);
	CHECK_EQ(cpu.reg().a(), 0x0F);
}

TEST_CASE("sub_a_n8 updates flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().a() = 0x0F;
	memory[1] = 0xF0;

	opcodes::sub_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

	cpu.pc() = 0;
	cpu.reg().a() = 0xF0;

	opcodes::sub_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("sub_a_n8 increments program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::sub_a_n8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("adc_a_r8 updates value properly", test, adc_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().c_flag() = true;

	test::reg(cpu) = 0xFE;
	test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0xFF);
}

TEST_CASE_TEMPLATE("adc_a_r8 increments program counter", test, adc_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("adc_a_r8 updates flags properly", test, adc_a_r8_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 1;
	cpu.reg().c_flag() = false;
	test::reg(cpu) = 0x0F;

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 1;
	cpu.reg().c_flag() = true;
	test::reg(cpu) = 0xFE;

	test::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("adc_a_a updates value properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().c_flag() = true;
	cpu.reg().a() = 0x0F;

	opcodes::adc_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0x1F);
}

TEST_CASE("adc_a_a increments program counter")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::adc_a_a::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("adc_a_a updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::adc_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0x0F;
	cpu.reg().c_flag() = false;

	opcodes::adc_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().a() = 0xFF;
	cpu.reg().c_flag() = true;

	opcodes::adc_a_a::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);
}

TEST_CASE("adc_a_n8 updates value properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().c_flag() = true;
	memory[1] = 0x0F;

	opcodes::adc_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().a(), 0x10);
}

TEST_CASE("adc_a_n8 increments program counter")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::adc_a_n8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("adc_a_n8 updates flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::adc_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	cpu.reg().a() = 0x0F;
	cpu.reg().c_flag() = false;
	memory[1] = 0x0F;

	opcodes::adc_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	cpu.reg().a() = 0xFF;
	cpu.reg().c_flag() = true;
	memory[1] = 0x0F;

	opcodes::adc_a_n8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("dec_hl decrements value")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().hl() = 0xABCD;
	memory[0xABCD] = 0xFF;
	opcodes::dec_hl::execute(cpu);

	CHECK_EQ(memory[0xABCD], 0xFE);
}

TEST_CASE("dec_hl increments program counter")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::dec_hl::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("dec_hl sets flags properly")
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.reg().hl() = 0xABCD;
	opcodes::dec_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), true);

	memory[0xABCD] = 0x2;
	opcodes::dec_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);

	opcodes::dec_hl::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), true);
	CHECK_EQ(cpu.reg().n_flag(), true);
	CHECK_EQ(cpu.reg().h_flag(), false);
}

TEST_CASE_TEMPLATE("add_hl_r16 updates register hl properly", test, add_hl_r16_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0);

	test::reg(cpu) = 0xF;
	test::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0xF);

	test::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0x1E);
}

TEST_CASE_TEMPLATE("add_hl_r16 updates flags properly", test, add_hl_r16_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::reg(cpu) = 0x0FFF;
	test::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::reg(cpu) = 0x1;
	test::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	test::reg(cpu) = 0xFFFF;
	test::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE_TEMPLATE("add_hl_r16 updates program counter properly", test, add_hl_r16_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("add_hl_hl updates register hl properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_hl_hl::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0);

	cpu.reg().hl() = 0xF;
	opcodes::add_hl_hl::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0x1E);

	opcodes::add_hl_hl::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0x3C);
}

TEST_CASE("add_hl_hl updates flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_hl_hl::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().hl() = 0x000F;
	opcodes::add_hl_hl::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().hl() = 0x0FFF;
	opcodes::add_hl_hl::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.reg().hl() = 0xEFFF;
	opcodes::add_hl_hl::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("add_hl_hl updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_hl_hl::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("add_hl_sp updates register hl properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_hl_sp::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0);

	cpu.sp() = 0xF;
	opcodes::add_hl_sp::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0xF);

	opcodes::add_hl_sp::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0x1E);
}

TEST_CASE("add_hl_sp updates flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_hl_sp::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.sp() = 0x0FFF;
	opcodes::add_hl_sp::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.sp() = 0x1;
	opcodes::add_hl_sp::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.sp() = 0xFFFF;
	opcodes::add_hl_sp::execute(cpu);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), true);

}

TEST_CASE("add_hl_sp updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_hl_sp::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("inc_sp increments stack pointer properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::inc_sp::execute(cpu);
	CHECK_EQ(cpu.sp(), 1);

	cpu.sp() = 0xFFFE;
	opcodes::inc_sp::execute(cpu);
	CHECK_EQ(cpu.sp(), 0xFFFF);

	opcodes::inc_sp::execute(cpu);
	CHECK_EQ(cpu.sp(), 0);
}

TEST_CASE("inc_sp updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::inc_sp::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("dec_sp decrements stack pointer properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = 0;
	opcodes::dec_sp::execute(cpu);
	CHECK_EQ(cpu.sp(), 0xFFFF);

	opcodes::dec_sp::execute(cpu);
	CHECK_EQ(cpu.sp(), 0xFFFE);

	cpu.sp() = 1;
	opcodes::dec_sp::execute(cpu);
	CHECK_EQ(cpu.sp(), 0);
}

TEST_CASE("dec_sp updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::dec_sp::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}
