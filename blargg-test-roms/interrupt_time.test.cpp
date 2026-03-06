
import blargg;
#include "doctest.h"

TEST_CASE("blargg.interrupt_time.interrupt_time")
{
	blargg::run_test("interrupt_time.gb", "interrupt_time\n\n\nPassed\n", 30e6);
}
