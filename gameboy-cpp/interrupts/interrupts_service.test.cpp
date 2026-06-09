

#include "doctest.h"

import cpu;
import std;
import tests;
import memory;
import interrupts;

namespace
{
	#define interrupt_test_cases \
		interrupts::vblank_interrupt, \
		interrupts::lcd_interrupt, \
		interrupts::timer_interrupt, \
		interrupts::serial_interrupt, \
		interrupts::joypad_interrupt

	template<typename T>
	T value_or_error(std::optional<T>&& optional, const std::string_view message)
	{
		REQUIRE_MESSAGE(optional.has_value(), message);
		return std::forward<std::optional<T>>(optional).value();
	}

	template<memory::Memory Memory, interrupts::InterruptServiceController InterruptController>
	void check_pending_interrupt(
		const interrupts::interrupt expected,
		cpu::cpu_state& cpu,
		Memory& memory,
		InterruptController& interrupts)
	{
		const auto actual = value_or_error(
			interrupts.get_first_pending(),
			"No interrupt pending");

		REQUIRE_EQ(actual, expected);
		tests::execute_complete_dispatch(actual, cpu, memory, interrupts);
	}
}

TEST_CASE("interrupts.Interrupts are not flagged as pending when enabled but not requested")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	interrupts::interrupt_controller controller{};
	controller.enable(interrupt);

	CHECK_FALSE(controller.is_pending(interrupt));
}

TEST_CASE("interrupts.Interrupts are not flagged as pending when requested but not enabled")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	interrupts::interrupt_controller controller{};
	controller.request(interrupt);

	CHECK_FALSE(controller.is_pending(interrupt));
}

TEST_CASE("interrupts.Interrupts are not flagged as pending when not requested nor enabled")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	interrupts::interrupt_controller controller{};
	controller.request(interrupt);

	CHECK_FALSE(controller.is_pending(interrupt));
}

TEST_CASE("interrupts.Interrupts are flagged as pending when enabled and requested")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	interrupts::interrupt_controller controller{};

	controller.enable(interrupt);
	controller.request(interrupt);

	CHECK(controller.is_pending(interrupt));
}

TEST_CASE("interrupts.Any serviceable interrupt is detected through is_any_interrupt_pending")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	interrupts::interrupt_controller controller{};

	controller.enable(interrupt);
	controller.request(interrupt);

	CHECK(controller.is_any_pending());
}

TEST_CASE("interrupts.Interrupts are serviced in order by priority")
{
	using namespace interrupts;

	cpu::cpu_state cpu{ };
	tests::test_memory memory{};
	interrupt_controller controller{};

	cpu.sp = 0xFFFE;

	// Enable all interrupts
	controller.enable(vblank_interrupt);
	controller.enable(lcd_interrupt);
	controller.enable(timer_interrupt);
	controller.enable(serial_interrupt);
	controller.enable(joypad_interrupt);

	// Request all interrupts
	controller.request(vblank_interrupt);
	controller.request(lcd_interrupt);
	controller.request(timer_interrupt);
	controller.request(serial_interrupt);
	controller.request(joypad_interrupt);

	// Assert
	check_pending_interrupt(vblank_interrupt, cpu, memory, controller);
	check_pending_interrupt(lcd_interrupt, cpu, memory, controller);
	check_pending_interrupt(timer_interrupt, cpu, memory, controller);
	check_pending_interrupt(serial_interrupt, cpu, memory, controller);
	check_pending_interrupt(joypad_interrupt, cpu, memory, controller);
}
