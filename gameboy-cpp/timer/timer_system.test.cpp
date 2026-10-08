
#include "doctest.h"

import std;
import timer;
import interrupts;

namespace
{
	template<std::integral auto N, std::invocable Function>
	void repeat(Function&& functor)
	{
		for (size_t i = 0; i < N; i++)
		{
			functor();
		}
	}

	template<timer::tac_clock Clock, std::integral auto TicksToIncrement>
	struct tac_clock_test_case
	{
		static constexpr auto ticks_to_increment = TicksToIncrement;
		static constexpr auto clock_select = Clock;
	};

	class test_interrupts
	{
	public:
		void request(const interrupts::interrupt& interrupt)
		{
			requests.insert(interrupt);
		}

		void clear_request(const interrupts::interrupt& interrupt)
		{
			requests.erase(interrupt);
		}

		[[nodiscard]] bool is_requested(const interrupts::interrupt& interrupt) const
		{
			return requests.contains(interrupt);
		}

	private:
		std::flat_set<interrupts::interrupt> requests {};
	};

#define tac_clock_test_cases \
    tac_clock_test_case<timer::tac_clock::_00, 256 * 4>, \
    tac_clock_test_case<timer::tac_clock::_01, 4 * 4>, \
    tac_clock_test_case<timer::tac_clock::_10, 16 * 4>, \
    tac_clock_test_case<timer::tac_clock::_11, 64 * 4>
}

TEST_CASE("timers.Divider register increases correctly with tac enabled")
{
	test_interrupts interrupts {};
	timer::timer_system timers{ };

	timers.set_control_enabled(true, interrupts);

	auto tick_timer = [&] { timers.tick(1, interrupts); };

	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0x00);

	repeat<0xF>(tick_timer);
	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0x0F);

	repeat<0xE1>(tick_timer);
	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0xF0);
}

TEST_CASE("timers.Divider register increases correctly with tac disabled")
{
	test_interrupts interrupts {};
	timer::timer_system timers{ };

	timers.set_control_enabled(false, interrupts);

	auto tick_timer = [&] { timers.tick(1, interrupts); };

	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0x00);

	repeat<0xF>(tick_timer);
	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0x0F);

	repeat<0xE1>(tick_timer);
	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0xF0);
}

TEST_CASE_TEMPLATE("timers.Tima is incremented properly based on clock selected", test, tac_clock_test_cases)
{
	test_interrupts interrupts {};
	timer::timer_system timers{ };

	timers.set_control_enabled(true, interrupts);
	timers.set_control_clock(test::clock_select);

	auto tick_timer = [&] { timers.tick(1, interrupts); };
	CHECK_EQ(timers.get_counter(), 0x00);

	repeat<test::ticks_to_increment>(tick_timer);
	CHECK_EQ(timers.get_counter(), 0x01);

	repeat<test::ticks_to_increment * 5>(tick_timer);
	CHECK_EQ(timers.get_counter(), 0x06);
}

TEST_CASE_TEMPLATE("timers.When tima overflows an interrupt is requested after an m-cycle", test, tac_clock_test_cases)
{
	test_interrupts interrupts {};
	timer::timer_system timers{ };

	timers.set_modulo(0xAB);
	timers.set_control_enabled(true, interrupts);
	timers.set_control_clock(test::clock_select);

	auto tick_timer = [&] { timers.tick(1, interrupts); };

	constexpr size_t ticks_to_overflow 
		= test::ticks_to_increment 
		* (std::numeric_limits<std::uint8_t>::max() + 1);

	repeat<ticks_to_overflow>(tick_timer);
	CHECK_EQ(timers.get_counter(), 0x0);
	CHECK_FALSE(interrupts.is_requested(interrupts::timer_interrupt));

	// TIMA stays at zero for a whole m-cycle (4 t-cycles)
	tick_timer();
	CHECK_EQ(timers.get_counter(), 0x0);
	CHECK_FALSE(interrupts.is_requested(interrupts::timer_interrupt));

	tick_timer();
	CHECK_EQ(timers.get_counter(), 0x0);
	CHECK_FALSE(interrupts.is_requested(interrupts::timer_interrupt));

	tick_timer();
	CHECK_EQ(timers.get_counter(), 0x0);
	CHECK_FALSE(interrupts.is_requested(interrupts::timer_interrupt));

	// TIMA is set to modulo and an interrupt is requested in the next m-cycle
	tick_timer();
	CHECK_EQ(timers.get_counter(), 0xAB);
	CHECK(interrupts.is_requested(interrupts::timer_interrupt));
}

TEST_CASE_TEMPLATE("timers.Tima does not increment when tac is disabled", test, tac_clock_test_cases)
{
	test_interrupts interrupts {};
	timer::timer_system timers{ };

	timers.set_control_enabled(false, interrupts);
	timers.set_control_clock(test::clock_select);

	auto tick_timer = [&] { timers.tick(1, interrupts); };

	repeat<test::ticks_to_increment>(tick_timer);
	CHECK_EQ(timers.get_counter(), 0x0);
	CHECK_FALSE(interrupts.is_requested(interrupts::timer_interrupt));

	repeat<test::ticks_to_increment * 10>(tick_timer);
	CHECK_EQ(timers.get_counter(), 0x0);
	CHECK_FALSE(interrupts.is_requested(interrupts::timer_interrupt));
}