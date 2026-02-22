
#include "doctest.h"

import cpu;
import std;

TEST_CASE("Cycle default constructor initializes all at zero")
{
	using namespace cpu::literals;
	cpu::cpu_cycle cycle{};

	CHECK_EQ(cycle.m_cycle(), 0_m_cycle);
	CHECK_EQ(cycle.t_cycle(), 0_t_cycle);
}

TEST_CASE("Cycle increments t_cycle and m_cycle properly")
{
	using namespace cpu::literals;
	cpu::cpu_cycle cycle{};

	CHECK_EQ(cycle.m_cycle(), 0_m_cycle);
	CHECK_EQ(cycle.t_cycle(), 0_t_cycle);

	cycle++;
	CHECK_EQ(cycle.m_cycle(), 0_m_cycle);
	CHECK_EQ(cycle.t_cycle(), 1_t_cycle);

	cycle++;
	CHECK_EQ(cycle.m_cycle(), 0_m_cycle);
	CHECK_EQ(cycle.t_cycle(), 2_t_cycle);

	cycle++;
	CHECK_EQ(cycle.m_cycle(), 0_m_cycle);
	CHECK_EQ(cycle.t_cycle(), 3_t_cycle);

	cycle++;
	CHECK_EQ(cycle.m_cycle(), 1_m_cycle);
	CHECK_EQ(cycle.t_cycle(), 0_t_cycle);
}

TEST_CASE("Cycle reports end of m_cycles properly")
{
	using namespace cpu::literals;
	cpu::cpu_cycle cycle{};

	cycle++;
	cycle++;
	cycle++;
	CHECK(cycle.end_of_m_cycle());
}
