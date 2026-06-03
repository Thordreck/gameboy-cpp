
#include "doctest.h"

import cpu;
import std;
import tests;
import interrupts;

namespace
{
	#define request_test_cases \
		interrupts::vblank_interrupt, \
		interrupts::lcd_interrupt, \
		interrupts::timer_interrupt, \
		interrupts::serial_interrupt, \
		interrupts::joypad_interrupt
}

TEST_CASE("interrupts.Requested interrupts have their if flag set")
{
	constexpr std::uint16_t if_address = 0xFF0F;
	const auto interrupt = GENERATE(request_test_cases);

	tests::test_memory memory{};
	interrupts::request(interrupt, memory);

	CHECK_EQ(memory.read(if_address) & interrupt.if_flag(), interrupt.if_flag());
}

TEST_CASE("interrupts.Unrequestd interrupts have their flag unset")
{
	constexpr std::uint16_t if_address = 0xFF0F;
	const auto interrupt = GENERATE(request_test_cases);

	tests::test_memory memory{};

	interrupts::request(interrupt, memory);
	interrupts::clear_request(interrupt, memory);

	CHECK_EQ(memory.read(if_address) & interrupt.if_flag(), 0x0);
}

TEST_CASE("interrupts.Requested interrupts are detected as is_requested")
{
	const auto interrupt = GENERATE(request_test_cases);

	tests::test_memory memory{};
	interrupts::request(interrupt, memory);

	CHECK(interrupts::is_requested(interrupt, memory));
}

TEST_CASE("interrupts.Unrequested interrupts are not detected as is_requested")
{
	const auto interrupt = GENERATE(request_test_cases);

	tests::test_memory memory{};

	interrupts::clear_request(interrupt, memory);
	CHECK_FALSE(interrupts::is_requested(interrupt, memory));
}
