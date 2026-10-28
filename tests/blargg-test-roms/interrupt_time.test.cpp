
import std;
import blargg;

#include "doctest.h"

TEST_CASE("blargg.interrupt_time.interrupt_time")
{
	blargg::run_lcd_test(
		"roms/interrupt-time/interrupt_time.gb",
		"reference/blargg_dmg_interrupt_time.png",
		static_cast<std::uint32_t>(30e6));
}
