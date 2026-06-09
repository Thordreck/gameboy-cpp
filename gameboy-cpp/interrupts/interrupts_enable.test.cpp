
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
	using namespace interrupts;
	const auto interrupt = GENERATE(enable_test_cases);

	interrupt_controller controller {};
	controller.enable(interrupt);

	CHECK_EQ(read_ie_register(controller) & interrupt.ie_flag(), interrupt.ie_flag());
}

TEST_CASE("interrupts.Disabled interrupts have their flag unset")
{
	using namespace interrupts;
	const auto interrupt = GENERATE(enable_test_cases);

	interrupt_controller controller {};
	controller.enable(interrupt);
	controller.disable(interrupt);

	CHECK_EQ(read_ie_register(controller) & interrupt.ie_flag(), 0x0);
}

TEST_CASE("interrupts.Enabled interrupts are detected as is_enabled")
{
	using namespace interrupts;
	const auto interrupt = GENERATE(enable_test_cases);

	interrupt_controller controller {};
	controller.enable(interrupt);

	CHECK(controller.is_enabled(interrupt));
}

TEST_CASE("interrupts.Disabled interrupts are not detected as is_enabled")
{
	using namespace interrupts;
	const auto interrupt = GENERATE(enable_test_cases);

	interrupt_controller controller {};
	controller.enable(interrupt);
	controller.disable(interrupt);

	CHECK_FALSE(controller.is_enabled(interrupt));
}
