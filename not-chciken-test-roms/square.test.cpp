import not_chciken;
#include "doctest.h"

TEST_CASE("not_chicken.square.preset.3")
{
    not_chciken::run_square_preset_test("reference/square_test_preset_3.wav", 3);
}

/*
TEST_CASE("not_chicken.square.preset.6")
{
    not_chciken::run_square_preset_test("reference/square_test_preset_6.wav", 6);
}
*/
