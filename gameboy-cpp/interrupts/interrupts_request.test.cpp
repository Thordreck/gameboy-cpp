
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
	using namespace interrupts;
	const auto interrupt = GENERATE(request_test_cases);

	interrupt_controller controller {};
	controller.request(interrupt);

	CHECK_EQ(read_if_register(controller) & interrupt.if_flag(), interrupt.if_flag());
}

TEST_CASE("interrupts.Unrequestd interrupts have their flag unset")
{
	using namespace interrupts;
	const auto interrupt = GENERATE(request_test_cases);

	interrupt_controller controller {};
	controller.request(interrupt);
	controller.clear_request(interrupt);

	CHECK_EQ(read_if_register(controller) & interrupt.if_flag(), 0);
}

TEST_CASE("interrupts.Requested interrupts are detected as is_requested")
{
	using namespace interrupts;
	const auto interrupt = GENERATE(request_test_cases);

	interrupt_controller controller {};
	controller.request(interrupt);

	CHECK(controller.is_requested(interrupt));
}

TEST_CASE("interrupts.Unrequested interrupts are not detected as is_requested")
{
	using namespace interrupts;
	const auto interrupt = GENERATE(request_test_cases);

	interrupt_controller controller {};
	controller.request(interrupt);
	controller.clear_request(interrupt);

	CHECK_FALSE(controller.is_requested(interrupt));
}
