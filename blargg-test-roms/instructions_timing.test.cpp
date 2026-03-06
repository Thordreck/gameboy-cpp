
import blargg;
#include "doctest.h"

TEST_CASE("blargg.instr_timing.instr_timing")
{
	blargg::run_test("instr_timing.gb", "instr_timing\n\n\nPassed\n", 30e5);
}
