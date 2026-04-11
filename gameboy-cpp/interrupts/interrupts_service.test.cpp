

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
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	interrupts::enable<typename test::interrupt_t>(cpu.memory());
	CHECK_FALSE(interrupts::is_pending<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Interrupts are not flagged as pending when requested but not enabled", test, interrupt_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	interrupts::request<typename test::interrupt_t>(cpu.memory());
	CHECK_FALSE(interrupts::is_pending<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Interrupts are not flagged as pending when not requested nor enabled", test, interrupt_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	CHECK_FALSE(interrupts::is_pending<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Interrupts are flagged as pending when enabled and requested", test, interrupt_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	interrupts::enable<typename test::interrupt_t>(cpu.memory());
	interrupts::request<typename test::interrupt_t>(cpu.memory());
	CHECK(interrupts::is_pending<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Any serviceable interrupt is detected through is_any_interrupt_pending", test, interrupt_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	interrupts::enable<typename test::interrupt_t>(cpu.memory());
	interrupts::request<typename test::interrupt_t>(cpu.memory());
	CHECK(interrupts::is_any_interrupt_pending(cpu));
}

TEST_CASE("interrupts.Interrupts are serviced in order by priority")
{
	using namespace interrupts;

	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);
	cpu.sp() = 0xFFFE;

	// Enable all interrupts
	enable<vblank_interrupt>(cpu.memory());
	enable<lcd_interrupt>(cpu.memory());
	enable<timer_interrupt>(cpu.memory());
	enable<serial_interrupt>(cpu.memory());
	enable<joypad_interrupt>(cpu.memory());

	// Request all interrupts
	request<vblank_interrupt>(cpu.memory());
	request<lcd_interrupt>(cpu.memory());
	request<timer_interrupt>(cpu.memory());
	request<serial_interrupt>(cpu.memory());
	request<joypad_interrupt>(cpu.memory());

	// Vblank
	cpu.cycle() = {};
	std::optional<interrupt_dispatcher> vblank_dispatcher = service_first_pending_interrupt(cpu);
	REQUIRE(vblank_dispatcher.has_value());

	tests::execute_complete_dispatch(vblank_dispatcher.value(), cpu);
	CHECK_EQ(cpu.pc().value(), vblank_interrupt::handler_address);

	// LCD
	cpu.cycle() = {};
	std::optional<interrupt_dispatcher> lcd_dispatcher = service_first_pending_interrupt(cpu);
	REQUIRE(lcd_dispatcher.has_value());

	tests::execute_complete_dispatch(lcd_dispatcher.value(), cpu);
	CHECK_EQ(cpu.pc().value(), lcd_interrupt::handler_address);

	// Timer
	cpu.cycle() = {};
	std::optional<interrupt_dispatcher> timer_dispatcher =service_first_pending_interrupt(cpu);
	REQUIRE(timer_dispatcher.has_value());

	tests::execute_complete_dispatch(timer_dispatcher.value(), cpu);
	CHECK_EQ(cpu.pc().value(), timer_interrupt::handler_address);

	// Serial
	cpu.cycle() = {};
	std::optional<interrupt_dispatcher> serial_dispatcher = service_first_pending_interrupt(cpu);
	REQUIRE(serial_dispatcher.has_value());

	tests::execute_complete_dispatch(serial_dispatcher.value(), cpu);
	CHECK_EQ(cpu.pc().value(), serial_interrupt::handler_address);

	// Joypad
	cpu.cycle() = {};
	std::optional<interrupt_dispatcher> joypad_dispatcher = service_first_pending_interrupt(cpu);
	REQUIRE(joypad_dispatcher.has_value());

	tests::execute_complete_dispatch(joypad_dispatcher.value(), cpu);
	CHECK_EQ(cpu.pc().value(), joypad_interrupt::handler_address);

	CHECK_FALSE(is_any_interrupt_pending(cpu));
	CHECK_EQ(service_first_pending_interrupt(cpu), std::nullopt);
}
