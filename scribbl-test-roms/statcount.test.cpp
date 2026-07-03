import scribbl;
#include "doctest.h"

TEST_CASE("scribbl.statcount")
{
    scribbl::run_lcd_test("roms/statcount-auto.gb", "reference/statcount-auto-pass.png", 480);
}

TEST_CASE("scribbl.lycscx")
{
    scribbl::run_lcd_test("roms/lycscx.gb", "reference/lycscx_expected.png", 480);
}

TEST_CASE("scribbl.lycscy")
{
    scribbl::run_lcd_test("roms/lycscy.gb", "reference/lycscy_expected.png", 480);
}
