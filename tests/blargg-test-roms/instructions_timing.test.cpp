
import std;
import blargg;

#include "doctest.h"

TEST_CASE("blargg.instr_timing.instr_timing")
{
	blargg::run_serial_test(
		"roms/instructions-timing/instr_timing.gb",
		"instr_timing\n\n\nPassed\n",
		static_cast<std::uint32_t>(30e5));
}
