
#include "doctest.h"

import std;
import timer;
import memory;
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

#define tac_clock_test_cases \
    tac_clock_test_case<timer::tac_clock::_00, 256 * 4>, \
    tac_clock_test_case<timer::tac_clock::_01, 4 * 4>, \
    tac_clock_test_case<timer::tac_clock::_10, 16 * 4>, \
    tac_clock_test_case<timer::tac_clock::_11, 64 * 4>
}

TEST_CASE("timers.Divider register increases correctly with tac enabled")
{
	std::array<memory::memory_data_t, memory::memory_size> memory{};
	auto memory_page = memory::map(memory);
	auto memory_map = memory::build_memory_map(memory_page);
	memory::memory_bus bus{ memory_map };

	timer::timer_system timers{ };
	timers.control().enabled = true;

	memory::connect(bus, timers);

	auto tick_timer = [&timers] { timers.tick(); };

	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0x00);

	repeat<0xF>(tick_timer);
	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0x0F);

	repeat<0xE1>(tick_timer);
	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0xF0);
}

TEST_CASE("timers.Divider register increases correctly with tac disabled")
{
	std::array<memory::memory_data_t, memory::memory_size> memory{};
	auto memory_page = memory::map(memory);
	auto memory_map = memory::build_memory_map(memory_page);
	memory::memory_bus bus{ memory_map };

	timer::timer_system timers{ };
	timers.control().enabled = false;

	memory::connect(bus, timers);

	auto tick_timer = [&timers] { timers.tick(); };

	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0x00);

	repeat<0xF>(tick_timer);
	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0x0F);

	repeat<0xE1>(tick_timer);
	CHECK_EQ(static_cast<std::uint16_t>(timers.divider()), 0xF0);
}

TEST_CASE_TEMPLATE("timers.Tima is incremented properly based on clock selected", test, tac_clock_test_cases)
{
	std::array<memory::memory_data_t, memory::memory_size> memory{};
	auto memory_page = memory::map(memory);
	auto memory_map = memory::build_memory_map(memory_page);
	memory::memory_bus bus{ memory_map };

	timer::timer_system timers{ };
	timers.control().enabled = true;
	timers.control().clock = test::clock_select;

	memory::connect(bus, timers);

	auto tick_timer = [&timers] { timers.tick(); };
	CHECK_EQ(timers.counter(), 0x00);

	repeat<test::ticks_to_increment>(tick_timer);
	CHECK_EQ(timers.counter().value(), 0x01);

	repeat<test::ticks_to_increment * 5>(tick_timer);
	CHECK_EQ(timers.counter().value(), 0x06);
}

TEST_CASE_TEMPLATE("timers.When tima overflows an interrupt is requested after an m-cycle", test, tac_clock_test_cases)
{
	std::array<memory::memory_data_t, memory::memory_size> memory{};
	auto memory_page = memory::map(memory);
	auto memory_map = memory::build_memory_map(memory_page);
	memory::memory_bus bus{ memory_map };

	timer::timer_system timers{ };
	timers.modulo().value = 0xAB;
	timers.control().enabled = true;
	timers.control().clock = test::clock_select;

	memory::connect(bus, timers);
	auto tick_timer = [&timers] { timers.tick(); };

	constexpr size_t ticks_to_overflow 
		= test::ticks_to_increment 
		* (std::numeric_limits<std::uint8_t>::max() + 1);

	repeat<ticks_to_overflow>(tick_timer);
	CHECK_EQ(timers.counter().value(), 0x0);
	CHECK_FALSE(interrupts::is_requested<interrupts::timer_interrupt>(bus));

	// TIMA stays at zero for a whole m-cycle (4 t-cycles)
	tick_timer();
	CHECK_EQ(timers.counter().value(), 0x0);
	CHECK_FALSE(interrupts::is_requested<interrupts::timer_interrupt>(bus));

	tick_timer();
	CHECK_EQ(timers.counter().value(), 0x0);
	CHECK_FALSE(interrupts::is_requested<interrupts::timer_interrupt>(bus));

	tick_timer();
	CHECK_EQ(timers.counter().value(), 0x0);
	CHECK_FALSE(interrupts::is_requested<interrupts::timer_interrupt>(bus));

	// TIMA is set to modulo and an interrupt is requested in the next m-cycle
	tick_timer();
	CHECK_EQ(timers.counter().value(), 0xAB);
	CHECK(interrupts::is_requested<interrupts::timer_interrupt>(bus));
}

TEST_CASE_TEMPLATE("timers.Tima does not increment when tac is disabled", test, tac_clock_test_cases)
{
	std::array<memory::memory_data_t, memory::memory_size> memory{};
	auto memory_page = memory::map(memory);
	auto memory_map = memory::build_memory_map(memory_page);
	memory::memory_bus bus{ memory_map };

	timer::timer_system timers{ };
	timers.control().enabled = false;
	timers.control().clock = test::clock_select;

	memory::connect(bus, timers);
	auto tick_timer = [&timers] { timers.tick(); };

	repeat<test::ticks_to_increment>(tick_timer);
	CHECK_EQ(timers.counter().value(), 0x0);
	CHECK_FALSE(interrupts::is_requested<interrupts::timer_interrupt>(bus));

	repeat<test::ticks_to_increment * 10>(tick_timer);
	CHECK_EQ(timers.counter().value(), 0x0);
	CHECK_FALSE(interrupts::is_requested<interrupts::timer_interrupt>(bus));
}