
#include "doctest.h"

import cpu;
import std;
import tests;
import memory;
import interrupts;

namespace
{
	#define dispatch_test_cases \
		interrupts::vblank_interrupt, \
		interrupts::lcd_interrupt, \
		interrupts::timer_interrupt, \
		interrupts::serial_interrupt, \
		interrupts::joypad_interrupt
}

TEST_CASE("interrupts.IME flag is disabled when interrupt is dispatched")
{
	const auto interrupt = GENERATE(dispatch_test_cases);

	cpu::cpu_state cpu{ };
	tests::test_memory memory{};
	interrupts::interrupt_controller controller{};

	cpu.sp = 0xFFFE;
	cpu.ime.enabled = true;

	controller.enable(interrupt);
	controller.request(interrupt);

	tests::execute_complete_dispatch(interrupt, cpu, memory, controller);

	CHECK_FALSE(cpu.ime.enabled);
	CHECK_FALSE(cpu.ime.requested);
}

TEST_CASE("interrupts.IF flag is unset when interrupt is dispatched")
{
	const auto interrupt = GENERATE(dispatch_test_cases);
	constexpr std::uint16_t if_address = 0xFF0F;

	cpu::cpu_state cpu{ };
	tests::test_memory memory{};
	interrupts::interrupt_controller controller{};

	cpu.sp = 0xFFFE;

	controller.enable(interrupt);
	controller.request(interrupt);

	tests::execute_complete_dispatch(interrupt, cpu, memory, controller);
	CHECK_FALSE(controller.is_pending(interrupt));
}

TEST_CASE("interrupts.PC is set to handler when interrupt is dispatched")
{
	const auto interrupt = GENERATE(dispatch_test_cases);

	cpu::cpu_state cpu{ };
	tests::test_memory memory{};
	interrupts::interrupt_controller controller{};

	cpu.sp = 0xFFFE;

	controller.enable(interrupt);
	controller.request(interrupt);

	tests::execute_complete_dispatch(interrupt, cpu, memory, controller);
	CHECK_EQ(cpu.pc, interrupt.handler_address());
}

TEST_CASE("interrupts.Previous pc is push to stack when interrupt is dispatched")
{
	const auto interrupt = GENERATE(dispatch_test_cases);
	constexpr std::uint16_t stack_origin = 0xFFFE;

	cpu::cpu_state cpu{ };
	tests::test_memory memory{};
	interrupts::interrupt_controller controller{};

	cpu.sp = stack_origin;
	cpu.pc = 0xABCD;

	controller.enable(interrupt);
	controller.request(interrupt);

	tests::execute_complete_dispatch(interrupt, cpu, memory, controller);

	CHECK_EQ(memory.read(stack_origin - 1), 0xAB);
	CHECK_EQ(memory.read(stack_origin - 2), 0xCD);
	CHECK_EQ(cpu.sp, stack_origin - 2);
}

TEST_CASE("interrupts.Dispatch takes 4 machine cycles")
{
	CHECK_EQ(interrupts::interrupt_dispatcher::num_steps(), 4);
}
