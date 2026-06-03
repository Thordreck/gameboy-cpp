

#include "doctest.h"

import cpu;
import std;
import tests;
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

	template<interrupts::InterruptDescriptor Interrupt, memory::Memory Memory>
	void check_pending_interrupt(const Interrupt& expected_interrupt, cpu::cpu_state& cpu, Memory& memory)
	{
		const auto pending_interrupt = value_or_error(
			interrupts::get_first_pending_interrupt(memory),
			"No interrupt pending");

		REQUIRE_EQ(pending_interrupt, expected_interrupt);
		tests::execute_complete_dispatch(pending_interrupt, cpu, memory);
	}
}

TEST_CASE("interrupts.Interrupts are not flagged as pending when enabled but not requested")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	tests::test_memory memory{};
	interrupts::enable(interrupt, memory);

	CHECK_FALSE(interrupts::is_pending(interrupt, memory));
}

TEST_CASE("interrupts.Interrupts are not flagged as pending when requested but not enabled")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	tests::test_memory memory{};
	interrupts::request(interrupt, memory);

	CHECK_FALSE(interrupts::is_pending(interrupt, memory));
}

TEST_CASE("interrupts.Interrupts are not flagged as pending when not requested nor enabled")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	tests::test_memory memory{};
	CHECK_FALSE(interrupts::is_pending(interrupt, memory));
}

TEST_CASE("interrupts.Interrupts are flagged as pending when enabled and requested")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	tests::test_memory memory{};

	interrupts::enable(interrupt, memory);
	interrupts::request(interrupt, memory);

	CHECK(interrupts::is_pending(interrupt, memory));
}

TEST_CASE("interrupts.Any serviceable interrupt is detected through is_any_interrupt_pending")
{
	const auto interrupt = GENERATE(interrupt_test_cases);

	tests::test_memory memory{};

	interrupts::enable(interrupt, memory);
	interrupts::request(interrupt, memory);

	CHECK(interrupts::is_any_interrupt_pending(memory));
}

TEST_CASE("interrupts.Interrupts are serviced in order by priority")
{
	using namespace tests;
	using namespace interrupts;

	cpu::cpu_state cpu{ };
	test_memory memory{};

	cpu.sp = 0xFFFE;

	// Enable all interrupts
	enable(vblank_interrupt, memory);
	enable(lcd_interrupt, memory);
	enable(timer_interrupt, memory);
	enable(serial_interrupt, memory);
	enable(joypad_interrupt, memory);

	// Request all interrupts
	request(vblank_interrupt, memory);
	request(lcd_interrupt, memory);
	request(timer_interrupt, memory);
	request(serial_interrupt, memory);
	request(joypad_interrupt, memory);

	// Assert
	check_pending_interrupt(vblank_interrupt, cpu, memory);
	check_pending_interrupt(lcd_interrupt, cpu, memory);
	check_pending_interrupt(timer_interrupt, cpu, memory);
	check_pending_interrupt(serial_interrupt, cpu, memory);
	check_pending_interrupt(joypad_interrupt, cpu, memory);
}
