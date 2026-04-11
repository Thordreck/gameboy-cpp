
#include "doctest.h"

import cpu;
import std;
import tests;
import interrupts;

namespace
{
	#define request_test_cases \
		tests::vblank_interrupt_test_case, \
		tests::lcd_interrupt_test_case, \
		tests::timer_interrupt_test_case, \
		tests::serial_interrupt_test_case, \
		tests::joypad_interrupt_test_case
}

TEST_CASE_TEMPLATE("interrupts.Requested interrupts have their if flag set", test, request_test_cases)
{
	constexpr std::uint16_t if_address = 0xFF0F;

	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	interrupts::request<typename test::interrupt_t>(cpu.memory());
	CHECK_EQ(memory.bus().read(if_address) & test::if_flag, test::if_flag);
}

TEST_CASE_TEMPLATE("interrupts.Unrequestd interrupts have their flag unset", test, request_test_cases)
{
	constexpr std::uint16_t if_address = 0xFF0F;

	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	interrupts::request<typename test::interrupt_t>(cpu.memory());
	interrupts::clear_request<typename test::interrupt_t>(cpu.memory());
	CHECK_EQ(memory.bus().read(if_address) & test::if_flag, 0x0);
}

TEST_CASE_TEMPLATE("interrupts.Requested interrupts are detected as is_requested", test, request_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	interrupts::request<typename test::interrupt_t>(cpu.memory());
	CHECK(interrupts::is_requested<typename test::interrupt_t>(cpu.memory()));
}

TEST_CASE_TEMPLATE("interrupts.Unrequested interrupts are not detected as is_requested", test, request_test_cases)
{
	cpu::cpu_state cpu{ };
	tests::mock_memory_bus memory{};

	memory::connect(memory.bus(), cpu);

	interrupts::clear_request<typename test::interrupt_t>(cpu.memory());
	CHECK_FALSE(interrupts::is_requested<typename test::interrupt_t>(cpu.memory()));
}
