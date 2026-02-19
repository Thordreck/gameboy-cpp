
#include "doctest.h"

import cpu;
import std;
import opcodes;

namespace
{
    template <auto RegFn>
    concept R16RegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_16>;
    };

	inline auto get_af(cpu::cpu& cpu) { return cpu.reg().af(); }
	inline auto get_bc(cpu::cpu& cpu) { return cpu.reg().bc(); }
	inline auto get_de(cpu::cpu& cpu) { return cpu.reg().de(); }
	inline auto get_hl(cpu::cpu& cpu) { return cpu.reg().hl(); }

	template<opcodes::Instruction OpCode, auto RegFn>
    requires R16RegisterFetchFn<RegFn>
	struct r16_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static auto reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	#define pop_r16_test_cases \
    r16_test_case<opcodes::pop_bc, get_bc>, \
    r16_test_case<opcodes::pop_de, get_de>, \
    r16_test_case<opcodes::pop_hl, get_hl>

	#define push_r16_test_cases \
    r16_test_case<opcodes::push_bc, get_bc>, \
    r16_test_case<opcodes::push_de, get_de>, \
    r16_test_case<opcodes::push_hl, get_hl>
}

TEST_CASE_TEMPLATE("pop_r16 updates reg with correct value from stack", test, pop_r16_test_cases)
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = stack_origin - 2;
	cpu.memory()[stack_origin - 2] = 0xCD;
	cpu.memory()[stack_origin - 1] = 0xAB;

	test::execute(cpu);

	CHECK_EQ(static_cast<std::uint16_t>(test::reg(cpu)), test_value);
	CHECK_EQ(cpu.pc(), 1);
	CHECK_EQ(cpu.sp(), stack_origin);
}


TEST_CASE_TEMPLATE("push_r16 updates stack with correct value from register", test, push_r16_test_cases)
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };
	cpu.sp() = stack_origin;

	test::reg(cpu) = test_value;
	test::execute(cpu);

	CHECK_EQ(cpu.memory()[stack_origin - 2], 0xCD);
	CHECK_EQ(cpu.memory()[stack_origin - 1], 0xAB);
	CHECK_EQ(cpu.pc(), 1);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}


TEST_CASE("ld_sp_n16 updates sp with correct value from memory")
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	memory[1] = 0xCD;
	memory[2] = 0xAB;

	opcodes::ld_sp_n16::execute(cpu);

	CHECK_EQ(cpu.sp(), test_value);
	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE("add_sp_e8 updates stack pointer properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = 0;
	memory[1] = 127;

	opcodes::add_sp_e8::execute(cpu);
	CHECK_EQ(cpu.sp(), 127);

	cpu.pc() = 0;
	memory[1] = -127;

	opcodes::add_sp_e8::execute(cpu);
	CHECK_EQ(cpu.sp(), 0x0);
}

TEST_CASE("add_sp_e8 updates flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = 0;
	memory[1] = 0xF;
	opcodes::add_sp_e8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	memory[1] = 0xF;
	opcodes::add_sp_e8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	memory[1] = 0xFF;
	opcodes::add_sp_e8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("add_sp_e8 updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::add_sp_e8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("ld_hl_sp_e8 updates hl properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = 127;
	memory[1] = 0;

	opcodes::ld_hl_sp_e8::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 127);

	cpu.pc() = 0;
	cpu.sp() = 127;
	memory[1] = -127;

	opcodes::ld_hl_sp_e8::execute(cpu);
	CHECK_EQ(cpu.reg().hl(), 0x0);
}

TEST_CASE("ld_hl_sp_e8 updates flags properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = 0;
	memory[1] = 0xF;
	opcodes::ld_hl_sp_e8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), false);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	cpu.sp() = 0xF;
	memory[1] = 0xF;
	opcodes::ld_hl_sp_e8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), false);

	cpu.pc() = 0;
	cpu.sp() = 0xFFFF;
	memory[1] = 0xFF;
	opcodes::ld_hl_sp_e8::execute(cpu);

	CHECK_EQ(cpu.reg().z_flag(), false);
	CHECK_EQ(cpu.reg().n_flag(), false);
	CHECK_EQ(cpu.reg().h_flag(), true);
	CHECK_EQ(cpu.reg().c_flag(), true);
}

TEST_CASE("ld_hl_sp_e8 updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	opcodes::ld_hl_sp_e8::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("pop_af updates reg with correct value from stack")
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = stack_origin - 2;
	cpu.memory()[stack_origin - 2] = 0xCD;
	cpu.memory()[stack_origin - 1] = 0xAB;

	opcodes::pop_af::execute(cpu);

	CHECK_EQ(cpu.reg().af(), test_value & 0xFFF0);
	CHECK_EQ(cpu.pc(), 1);
	CHECK_EQ(cpu.sp(), stack_origin);
}

TEST_CASE("push_af updates stack with correct value from register")
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = stack_origin;
	cpu.reg().af() = test_value;

	opcodes::push_af::execute(cpu);

	CHECK_EQ(cpu.memory()[stack_origin - 2], 0xC0);
	CHECK_EQ(cpu.memory()[stack_origin - 1], 0xAB);
	CHECK_EQ(cpu.pc(), 1);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}
