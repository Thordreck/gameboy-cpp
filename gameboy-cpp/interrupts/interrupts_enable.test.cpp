
#include "doctest.h"

import cpu;
import std;
import tests;
import memory;
import interrupts;

namespace
{
	#define enable_test_cases \
		interrupts::vblank_interrupt, \
		interrupts::lcd_interrupt, \
		interrupts::timer_interrupt, \
		interrupts::serial_interrupt, \
		interrupts::joypad_interrupt
}

TEST_CASE("interrupts.Enabled interrupts have their ie flag set")
{
	constexpr std::uint16_t ie_address = 0xFFFF;
	const auto interrupt = GENERATE(enable_test_cases);

	tests::test_memory memory {};
	interrupts::enable(interrupt, memory);

	CHECK_EQ(memory.read(ie_address) & interrupt.ie_flag(), interrupt.ie_flag());
}

TEST_CASE("interrupts.Disabled interrupts have their flag unset")
{
	constexpr std::uint16_t ie_address = 0xFFFF;
	const auto interrupt = GENERATE(enable_test_cases);

	tests::test_memory memory{};

	interrupts::enable(interrupt, memory);
	interrupts::disable(interrupt, memory);

	CHECK_EQ(memory.read(ie_address) & interrupt.ie_flag(), 0x0);
}

TEST_CASE("interrupts.Enabled interrupts are detected as is_enabled")
{
	const auto interrupt = GENERATE(enable_test_cases);

	tests::test_memory memory{};

	interrupts::enable(interrupt, memory);
	CHECK(interrupts::is_enabled(interrupt, memory));
}

TEST_CASE("interrupts.Disabled interrupts are not detected as is_enabled")
{
	const auto interrupt = GENERATE(enable_test_cases);

	tests::test_memory memory{};

	interrupts::enable(interrupt, memory);
	interrupts::disable(interrupt, memory);

	CHECK_FALSE(interrupts::is_enabled(interrupt, memory));
}
