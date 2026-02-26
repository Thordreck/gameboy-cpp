

#include "doctest.h"

import cpu;
import std;
import tests;
import interrupts;

namespace
{
	#define interrupt_test_cases \
		tests::vblank_interrupt_test_case, \
		tests::lcd_interrupt_test_case, \
		tests::timer_interrupt_test_case, \
		tests::serial_interrupt_test_case, \
		tests::joypad_interrupt_test_case
}

TEST_CASE_TEMPLATE("interrupts.Interrupts are not flagged as pending when enabled but not requested", test, interrupt_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::enable<typename test::interrupt_t>(cpu);
	CHECK_FALSE(interrupts::is_pending<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Interrupts are not flagged as pending when requested but not enabled", test, interrupt_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::request<typename test::interrupt_t>(cpu);
	CHECK_FALSE(interrupts::is_pending<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Interrupts are not flagged as pending when not requested nor enabled", test, interrupt_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	CHECK_FALSE(interrupts::is_pending<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Interrupts are flagged as pending when enabled and requested", test, interrupt_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::enable<typename test::interrupt_t>(cpu);
	interrupts::request<typename test::interrupt_t>(cpu);
	CHECK(interrupts::is_pending<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Any serviceable interrupt is detected through is_any_interrupt_pending", test, interrupt_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::enable<typename test::interrupt_t>(cpu);
	interrupts::request<typename test::interrupt_t>(cpu);
	CHECK(interrupts::is_any_interrupt_pending(cpu));
}

TEST_CASE("interrupts.Interrupts are serviced in order by priority")
{
	using namespace interrupts;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };
	cpu.sp() = 0xFFFE;

	// Enable all interrupts
	enable<vblank_interrupt>(cpu);
	enable<lcd_interrupt>(cpu);
	enable<timer_interrupt>(cpu);
	enable<serial_interrupt>(cpu);
	enable<joypad_interrupt>(cpu);

	// Request all interrupts
	request<vblank_interrupt>(cpu);
	request<lcd_interrupt>(cpu);
	request<timer_interrupt>(cpu);
	request<serial_interrupt>(cpu);
	request<joypad_interrupt>(cpu);

	service_first_pending_interrupt(cpu);
	CHECK_EQ(cpu.pc().value(), vblank_interrupt::handler_address);

	service_first_pending_interrupt(cpu);
	CHECK_EQ(cpu.pc().value(), lcd_interrupt::handler_address);

	service_first_pending_interrupt(cpu);
	CHECK_EQ(cpu.pc().value(), timer_interrupt::handler_address);

	service_first_pending_interrupt(cpu);
	CHECK_EQ(cpu.pc().value(), serial_interrupt::handler_address);

	service_first_pending_interrupt(cpu);
	CHECK_EQ(cpu.pc().value(), joypad_interrupt::handler_address);

	CHECK_FALSE(is_any_interrupt_pending(cpu));
}
