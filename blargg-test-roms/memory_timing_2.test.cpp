
import blargg;
#include "doctest.h"

TEST_CASE("blargg.mem_timing-2.01-read_timing")
{
	blargg::run_test("01-read_timing-2.gb", "01-read_timing\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.mem_timing-2.02-write_timing")
{
	blargg::run_test("02-write_timing-2.gb", "02-write_timing\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.mem_timing-2.03-modify_timing")
{
	blargg::run_test("03-modify_timing-2.gb", "03-modify_timing\n\n\nPassed\n", 30e5);
}
