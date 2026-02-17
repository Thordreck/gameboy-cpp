
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
	struct pop_r16_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static auto reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	#define pop_r16_test_cases \
    pop_r16_test_case<opcodes::pop_af, get_af>, \
    pop_r16_test_case<opcodes::pop_bc, get_bc>, \
    pop_r16_test_case<opcodes::pop_de, get_de>, \
    pop_r16_test_case<opcodes::pop_hl, get_hl>

	template<opcodes::Instruction OpCode, auto RegFn>
    requires R16RegisterFetchFn<RegFn>
	struct push_r16_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static auto reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	#define push_r16_test_cases \
    push_r16_test_case<opcodes::push_af, get_af>, \
    push_r16_test_case<opcodes::push_bc, get_bc>, \
    push_r16_test_case<opcodes::push_de, get_de>, \
    push_r16_test_case<opcodes::push_hl, get_hl>
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

