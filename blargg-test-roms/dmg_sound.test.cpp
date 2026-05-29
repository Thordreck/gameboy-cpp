import blargg;
#include "doctest.h"

TEST_CASE("blargg.dmg_sound.01-registers")
{
    blargg::run_memory_test("01-registers.gb", "01-registers\n\n\nPassed\n", 30e6);
}

TEST_CASE("blargg.dmg_sound.02-len ctr")
{
    blargg::run_memory_test("02-len ctr.gb", "02-len ctr\n\n0 1 2 3 \nPassed\n", 60e6);
}

TEST_CASE("blargg.dmg_sound.03-trigger")
{
    blargg::run_memory_test("03-trigger.gb", "03-trigger\n\n\nPassed\n", 30e6);
}

TEST_CASE("blargg.dmg_sound.04-sweep")
{
    blargg::run_memory_test("04-sweep.gb", "04-sweep\n\n\nPassed\n", 30e6);
}

TEST_CASE("blargg.dmg_sound.05-sweep details")
{
    blargg::run_memory_test("05-sweep details.gb", "05-sweep details\n\n\nPassed\n", 60e5);
}

TEST_CASE("blargg.dmg_sound.06-overflow on trigger")
{
    blargg::run_memory_test("06-overflow on trigger.gb", "06-overflow on trigger\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.dmg_sound.07-len sweep period sync")
{
    blargg::run_memory_test("07-len sweep period sync.gb", "07-len sweep period sync\n\n\nPassed\n", 60e5);
}

TEST_CASE("blargg.dmg_sound.08-len ctr during power")
{
    blargg::run_memory_test("08-len ctr during power.gb", "08-len ctr during power\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.dmg_sound.09-wave read while on")
{
    blargg::run_memory_test("09-wave read while on.gb", "09-wave read while on\n\n\nPassed\n", 30e6);
}

TEST_CASE("blargg.dmg_sound.10-wave trigger while on")
{
    blargg::run_memory_test("10-wave trigger while on.gb", "10-wave trigger while on\n\n\nPassed\n",  30e6);
}

TEST_CASE("blargg.dmg_sound.11-regs after power")
{
    blargg::run_memory_test("11-regs after power.gb", "11-regs after power\n\n\nPassed\n", 30e6);
}

TEST_CASE("blargg.dmg_sound.12-wave write while on")
{
    blargg::run_memory_test("12-wave write while on.gb", "12-wave write while on\n\n\nPassed\n", 30e6);
}
