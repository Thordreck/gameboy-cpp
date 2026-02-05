#include "doctest.h"

import cpu;

TEST_CASE("af register splits high and low byte properly")
{
	cpu::registers registers{};
	CHECK(registers.af() == 0);

	registers.af() = 1;
	CHECK(registers.af() == 1);
	CHECK(registers.a() == 1);
	CHECK(registers.f() == 0);
}
