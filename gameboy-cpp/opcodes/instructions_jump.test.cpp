
#include "doctest.h"

import cpu;
import opcodes;
import std;

namespace
{
    template <auto FlagRegFn>
    concept FlagRegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { FlagRegFn(cpu) } -> std::convertible_to<cpu::flag_register>;
    };

	auto get_z(cpu::cpu& cpu) { return cpu.reg().z_flag(); }
	auto get_c(cpu::cpu& cpu) { return cpu.reg().c_flag(); }

	template <auto FlagRegFn, bool value>
	requires FlagRegisterFetchFn<FlagRegFn>
	struct flag_setter
	{
		static void set(cpu::cpu& cpu)
		{
			FlagRegFn(cpu) = value;
		}
	};

	template <auto FlagRegFn>
	using set_flag = flag_setter<FlagRegFn, true>;

	template <auto FlagRegFn>
	using unset_flag = flag_setter<FlagRegFn, false>;

	using set_z_flag = set_flag<get_z>;
	using unset_z_flag = unset_flag<get_z>;

	using set_c_flag = set_flag<get_c>;
	using unset_c_flag = unset_flag<get_c>;

	template <typename T>
	concept CpuStateConditionSetter = requires(cpu::cpu & cpu)
	{
		{ T::set(cpu) } -> std::same_as<void>;
	};

	template<opcodes::Instruction OpCode, CpuStateConditionSetter condition_setter>
	struct jump_cc_test_case
	{
		static constexpr auto execute = OpCode::execute;

		static void set_condition(cpu::cpu& cpu)
		{
			condition_setter::set(cpu);
		}
	};

	#define jp_cc_n16_successful_jump_test_cases \
    jump_cc_test_case<opcodes::jp_z_n16, set_z_flag>, \
    jump_cc_test_case<opcodes::jp_nz_n16, unset_z_flag>, \
    jump_cc_test_case<opcodes::jp_c_n16, set_c_flag>, \
    jump_cc_test_case<opcodes::jp_nc_n16, unset_c_flag>

	#define jp_cc_n16_unsuccessful_jump_test_cases \
    jump_cc_test_case<opcodes::jp_z_n16, unset_z_flag>, \
    jump_cc_test_case<opcodes::jp_nz_n16, set_z_flag>, \
    jump_cc_test_case<opcodes::jp_c_n16, unset_c_flag>, \
    jump_cc_test_case<opcodes::jp_nc_n16, set_c_flag>

	#define jr_cc_n16_successful_jump_test_cases \
    jump_cc_test_case<opcodes::jr_z_n16, set_z_flag>, \
    jump_cc_test_case<opcodes::jr_nz_n16, unset_z_flag>, \
    jump_cc_test_case<opcodes::jr_c_n16, set_c_flag>, \
    jump_cc_test_case<opcodes::jr_nc_n16, unset_c_flag>

	#define jr_cc_n16_unsuccessful_jump_test_cases \
    jump_cc_test_case<opcodes::jr_z_n16, unset_z_flag>, \
    jump_cc_test_case<opcodes::jr_nz_n16, set_z_flag>, \
    jump_cc_test_case<opcodes::jr_c_n16, unset_c_flag>, \
    jump_cc_test_case<opcodes::jr_nc_n16, set_c_flag>
}

TEST_CASE("jp_n16 updates pc correctly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;

	opcodes::jp_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE_TEMPLATE("jp_cc_n16 sets expected pc when condition is met", test, jp_cc_n16_successful_jump_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), 0XAAFF);
}

TEST_CASE_TEMPLATE("jp_cc_n16 sets expected pc when condition is not met", test, jp_cc_n16_unsuccessful_jump_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 0xFF;
	cpu.memory()[2] = 0xAA;

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE("jr_n16 updates pc correctly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 2;

	opcodes::jr_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 4);

	cpu.memory()[5] = -2;

	opcodes::jr_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 4);
}

TEST_CASE_TEMPLATE("jr_cc_n16 sets expected pc when condition is met", test, jr_cc_n16_successful_jump_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 2;

	test::set_condition(cpu);
	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 4);

	cpu.memory()[5] = -2;

	opcodes::jr_n16::execute(cpu);
	CHECK_EQ(cpu.pc(), 4);
}

TEST_CASE_TEMPLATE("jr_cc_n16 sets expected pc when condition is not met", test, jr_cc_n16_unsuccessful_jump_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.memory()[1] = 2;

	test::set_condition(cpu);
	test::execute(cpu);
	CHECK_EQ(cpu.pc(), 2);
}
