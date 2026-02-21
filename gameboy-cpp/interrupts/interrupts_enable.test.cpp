
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
		static constexpr std::uint8_t ie_flag = flag;
	};

	#define enable_test_cases \
		interrupt_test_case<vblank_interrupt, 0b00000001>, \
		interrupt_test_case<lcd_interrupt, 0b00000010>, \
		interrupt_test_case<timer_interrupt, 0b00000100>, \
		interrupt_test_case<serial_interrupt, 0b00001000>, \
		interrupt_test_case<joypad_interrupt, 0b00010000>
}

TEST_CASE_TEMPLATE("interrupts.Enabled interrupts have their ie flag set", test, enable_test_cases)
{
	constexpr std::uint16_t ie_address = 0xFFFF;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::enable<typename test::interrupt_t>(cpu);
	CHECK_EQ(memory[ie_address] & test::ie_flag, test::ie_flag);
}

TEST_CASE_TEMPLATE("interrupts.Disabled interrupts have their flag unset", test, enable_test_cases)
{
	constexpr std::uint16_t ie_address = 0xFFFF;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::enable<typename test::interrupt_t>(cpu);
	interrupts::disable<typename test::interrupt_t>(cpu);
	CHECK_EQ(memory[ie_address] & test::ie_flag, 0x0);
}

TEST_CASE_TEMPLATE("interrupts.Enabled interrupts are detected as is_enabled", test, enable_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::enable<typename test::interrupt_t>(cpu);
	CHECK(interrupts::is_enabled<typename test::interrupt_t>(cpu));
}

TEST_CASE_TEMPLATE("interrupts.Disabled interrupts are not detected as is_enabled", test, enable_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	interrupts::enable<typename test::interrupt_t>(cpu);
	interrupts::disable<typename test::interrupt_t>(cpu);
	CHECK_FALSE(interrupts::is_enabled<typename test::interrupt_t>(cpu));
}
