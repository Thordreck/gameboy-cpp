
#include "doctest.h"

import cpu;
import std;
import tests;
import memory;
import interrupts;

namespace
{
	#define dispatch_test_cases \
		tests::vblank_interrupt_test_case, \
		tests::lcd_interrupt_test_case, \
		tests::timer_interrupt_test_case, \
		tests::serial_interrupt_test_case, \
		tests::joypad_interrupt_test_case
}

TEST_CASE_TEMPLATE("interrupts.IME flag is disabled when interrupt is dispatched", test, dispatch_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	cpu.sp() = 0xFFFE;
	cpu.ime_flag().enable();
	tests::execute_complete_dispatch<typename test::interrupt_t>(cpu);

	CHECK_FALSE(cpu.ime_flag().is_enabled());
	CHECK_FALSE(cpu.ime_flag().is_requested());
}

TEST_CASE_TEMPLATE("interrupts.IF flag is unset when interrupt is dispatched", test, dispatch_test_cases)
{
	constexpr std::uint16_t if_address = 0xFF0F;

	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);
	cpu.sp() = 0xFFFE;

	const auto if_value = memory.bus().read(if_address);
	memory.bus().write(if_address, if_value | test::if_flag);
	tests::execute_complete_dispatch<typename test::interrupt_t>(cpu);

	CHECK_EQ(memory.bus().read(if_address) & test::if_flag, 0x0);
}

TEST_CASE_TEMPLATE("interrupts.PC is set to handler when interrupt is dispatched", test, dispatch_test_cases)
{
	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);
	cpu.sp() = 0xFFFE;

	tests::execute_complete_dispatch<typename test::interrupt_t>(cpu);
	CHECK_EQ(cpu.pc(), test::handler);
}

TEST_CASE_TEMPLATE("interrupts.Previous pc is push to stack when interrupt is dispatched", test, dispatch_test_cases)
{
	constexpr std::uint16_t stack_origin = 0xFFFE;

	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);
	cpu.sp() = stack_origin;
	cpu.pc() = 0xABCD;

	tests::execute_complete_dispatch<typename test::interrupt_t>(cpu);

	CHECK_EQ(memory.bus().read(stack_origin - 1), 0xAB);
	CHECK_EQ(memory.bus().read(stack_origin - 2), 0xCD);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}

TEST_CASE_TEMPLATE("interrupts.Dispatch takes 5 machine cycles", test, dispatch_test_cases)
{
	constexpr std::uint16_t stack_origin = 0xFFFE;

	cpu::cpu cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);
	cpu.sp() = stack_origin;
	cpu.pc() = 0xABCD;

	tests::execute_complete_dispatch<typename test::interrupt_t>(cpu);

	using namespace cpu::literals;
	CHECK_EQ(cpu.cycle().m_cycle(), 5_m_cycle);
}
