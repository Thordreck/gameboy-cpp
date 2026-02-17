
#include "doctest.h"

import cpu;
import std;
import opcodes;

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
	struct call_cc_n16_test_case
	{
		static constexpr auto execute = OpCode::execute;

		static void set_condition(cpu::cpu& cpu)
		{
			condition_setter::set(cpu);
		}
	};

	#define call_cc_n16_successful_jump_test_cases \
    call_cc_n16_test_case<opcodes::call_z_n16, set_z_flag>, \
    call_cc_n16_test_case<opcodes::call_nz_n16, unset_z_flag>, \
    call_cc_n16_test_case<opcodes::call_c_n16, set_c_flag>, \
    call_cc_n16_test_case<opcodes::call_nc_n16, unset_c_flag>

	#define call_cc_n16_unsuccessful_jump_test_cases \
    call_cc_n16_test_case<opcodes::call_z_n16, unset_z_flag>, \
    call_cc_n16_test_case<opcodes::call_nz_n16, set_z_flag>, \
    call_cc_n16_test_case<opcodes::call_c_n16, unset_c_flag>, \
    call_cc_n16_test_case<opcodes::call_nc_n16, set_c_flag>

	template<opcodes::Instruction OpCode, CpuStateConditionSetter condition_setter>
	struct ret_cc_test_case
	{
		static constexpr auto execute = OpCode::execute;

		static void set_condition(cpu::cpu& cpu)
		{
			condition_setter::set(cpu);
		}
	};

	#define ret_cc_successful_jump_test_cases \
    ret_cc_test_case<opcodes::ret_z, set_z_flag>, \
    ret_cc_test_case<opcodes::ret_nz, unset_z_flag>, \
    ret_cc_test_case<opcodes::ret_c, set_c_flag>, \
    ret_cc_test_case<opcodes::ret_nc, unset_c_flag>

	#define ret_cc_unsuccessful_jump_test_cases \
    ret_cc_test_case<opcodes::ret_z, unset_z_flag>, \
    ret_cc_test_case<opcodes::ret_nz, set_z_flag>, \
    ret_cc_test_case<opcodes::ret_c, unset_c_flag>, \
    ret_cc_test_case<opcodes::ret_nc, set_c_flag>
}

TEST_CASE("call_n16 pushes pc into stack and jumps to direction")
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t pc_start = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.pc() = pc_start;
	cpu.sp() = stack_origin;
	cpu.memory()[pc_start + 1] = 0xFF;
	cpu.memory()[pc_start + 2] = 0xAA;

	opcodes::call_n16::execute(cpu);

	CHECK_EQ(cpu.pc(), 0XAAFF);
	CHECK_EQ(cpu.memory()[stack_origin - 2], 0xD0);
	CHECK_EQ(cpu.memory()[stack_origin - 1], 0xAB);
}

TEST_CASE_TEMPLATE("call_cc_n16 executes jump when condition is met", test, call_cc_n16_successful_jump_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t pc_start = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.pc() = pc_start;
	cpu.sp() = stack_origin;
	cpu.memory()[pc_start + 1] = 0xFF;
	cpu.memory()[pc_start + 2] = 0xAA;

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), 0XAAFF);
	CHECK_EQ(cpu.memory()[stack_origin - 2], 0xD0);
	CHECK_EQ(cpu.memory()[stack_origin - 1], 0xAB);
}

TEST_CASE_TEMPLATE("call_cc_n16 does not execute jump when condition is met", test, call_cc_n16_unsuccessful_jump_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t pc_start = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.pc() = pc_start;
	cpu.sp() = stack_origin;
	cpu.memory()[pc_start + 1] = 0xFF;
	cpu.memory()[pc_start + 2] = 0xAA;

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), pc_start + 3);
	CHECK_EQ(cpu.memory()[stack_origin - 2], 0);
	CHECK_EQ(cpu.memory()[stack_origin - 1], 0);
}

TEST_CASE("ret updates pc with value from stack")
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t expected_pc = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = stack_origin - 2;
	cpu.memory()[stack_origin - 1] = 0xAB;
	cpu.memory()[stack_origin - 2] = 0xCD;

	opcodes::ret::execute(cpu);
	CHECK_EQ(cpu.pc(), expected_pc);
}

TEST_CASE_TEMPLATE("ret_cc updates pc if condition is satisfied", test, ret_cc_successful_jump_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t expected_pc = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = stack_origin - 2;
	cpu.memory()[stack_origin - 1] = 0xAB;
	cpu.memory()[stack_origin - 2] = 0xCD;

	test::set_condition(cpu);
	test::execute(cpu);
	CHECK_EQ(cpu.pc(), expected_pc);
}

TEST_CASE_TEMPLATE("ret_cc does not update pc if condition is not satisfied", test, ret_cc_unsuccessful_jump_test_cases)
{
	constexpr cpu::register_16::type_t stack_origin = 0xFFFE;
	constexpr cpu::program_counter::type_t expected_pc = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = stack_origin - 2;
	cpu.memory()[stack_origin - 1] = 0xAB;
	cpu.memory()[stack_origin - 2] = 0xCD;

	test::set_condition(cpu);
	test::execute(cpu);

	CHECK_EQ(cpu.pc(), 1);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}
