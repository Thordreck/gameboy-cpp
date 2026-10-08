
import blargg;
#include "doctest.h"

TEST_CASE("blargg.halt_bug.halt_bug")
{
	blargg::run_test("roms/halt_bug/halt_bug.gb", "halt_bug\n\n\nPassed\n", 30e5);
}
