
#include "doctest.h"

import cpu;
import std;
import interrupts;

namespace
{
	using namespace interrupts;

	template<InterruptDescriptor interrupt, std::uint8_t flag>
	struct interrupt_test_case
	{
		using interrupt_t = interrupt;
		static constexpr std::uint8_t if_flag = flag;
	};

	#define request_test_cases \
		interrupt_test_case<vblank_interrupt, 0b00000001>, \
		interrupt_test_case<lcd_interrupt, 0b00000010>, \
		interrupt_test_case<timer_interrupt, 0b00000100>, \
		interrupt_test_case<serial_interrupt, 0b00001000>, \
		interrupt_test_case<joypad_interrupt, 0b00010000>
}

TEST_CASE_TEMPLATE("interrupts.Requested interrupts have their if flag set", test, request_test_cases)
{
	constexpr std::uint16_t if_address = 0xFF0F;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::request<typename test::interrupt_t>(cpu);
	CHECK_EQ(memory[if_address] & test::if_flag, test::if_flag);
}

TEST_CASE_TEMPLATE("interrupts.Unrequestd interrupts have their flag unset", test, request_test_cases)
{
	constexpr std::uint16_t if_address = 0xFF0F;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::request<typename test::interrupt_t>(cpu);
	interrupts::clear_request<typename test::interrupt_t>(cpu);
	CHECK_EQ(memory[if_address] & test::if_flag, 0x0);
}

TEST_CASE_TEMPLATE("interrupts.Requested interrupts are detected as is_requested", test, request_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::request<typename test::interrupt_t>(cpu);
	CHECK(interrupts::is_requested<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Unrequested interrupts are not detected as is_requested", test, request_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::clear_request<typename test::interrupt_t>(cpu);
	CHECK_FALSE(interrupts::is_requested<typename test::interrupt_t>(cpu));
}
