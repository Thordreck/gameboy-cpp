
import blargg;
#include "doctest.h"

TEST_CASE("blargg.mem_timing.01-read_timing")
{
	blargg::run_test("01-read_timing.gb", "01-read_timing\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.mem_timing.02-write_timing")
{
	blargg::run_test("02-write_timing.gb", "02-write_timing\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.mem_timing.03-modify_timing")
{
	blargg::run_test("03-modify_timing.gb", "03-modify_timing\n\n\nPassed\n", 30e5);
}
