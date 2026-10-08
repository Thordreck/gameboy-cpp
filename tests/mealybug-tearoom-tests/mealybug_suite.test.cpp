import mealybug;
#include "doctest.h"

TEST_CASE("mealybug.m2_win_en_toggle")
{
    mealybug::run_test("roms/m2_win_en_toggle.gb", "reference/m2_win_en_toggle.png", 10);
}

TEST_CASE("mealybug.m3_bgp_change")
{
    mealybug::run_test("roms/m3_bgp_change.gb", "reference/m3_bgp_change.png", 10);
}

TEST_CASE("mealybug.m3_bgp_change_sprites")
{
    mealybug::run_test("roms/m3_bgp_change_sprites.gb", "reference/m3_bgp_change_sprites.png", 10);
}

TEST_CASE("mealybug.m3_lcdc_bg_en_change")
{
    mealybug::run_test("roms/m3_lcdc_bg_en_change.gb", "reference/m3_lcdc_bg_en_change.png", 10);
}

// TODO: reference image missing?
/*
TEST_CASE("mealybug.m3_lcdc_bg_en_change2")
{
    mealybug::run_test("roms/m3_lcdc_bg_en_change2.gb", "reference/m3_lcdc_bg_en_change_variant.png", 10);
}
*/

TEST_CASE("mealybug.m3_lcdc_bg_map_change")
{
    mealybug::run_test("roms/m3_lcdc_bg_map_change.gb", "reference/m3_lcdc_bg_map_change.png", 10);
}

// TODO: reference image missing?
/*
TEST_CASE("mealybug.m3_lcdc_bg_map_change2")
{
    mealybug::run_test("roms/m3_lcdc_bg_map_change2.gb", "reference/m3_lcdc_bg_map_change_variant.png", 10);
}
*/

TEST_CASE("mealybug.m3_lcdc_obj_en_change")
{
    mealybug::run_test("roms/m3_lcdc_obj_en_change.gb", "reference/m3_lcdc_obj_en_change.png", 10);
}

TEST_CASE("mealybug.m3_lcdc_obj_en_change_variant")
{
    mealybug::run_test("roms/m3_lcdc_obj_en_change_variant.gb", "reference/m3_lcdc_obj_en_change_variant.png", 10);
}

TEST_CASE("mealybug.m3_lcdc_obj_size_change")
{
    mealybug::run_test("roms/m3_lcdc_obj_size_change.gb", "reference/m3_lcdc_obj_size_change.png", 10);
}

TEST_CASE("mealybug.m3_lcdc_obj_size_change_scx")
{
    mealybug::run_test("roms/m3_lcdc_obj_size_change_scx.gb", "reference/m3_lcdc_obj_size_change_scx.png", 10);
}

TEST_CASE("mealybug.m3_lcdc_tile_sel_change")
{
    mealybug::run_test("roms/m3_lcdc_tile_sel_change.gb", "reference/m3_lcdc_tile_sel_change.png", 10);
}

// TODO: reference image missing?
/*
TEST_CASE("mealybug.m3_lcdc_tile_sel_change2")
{
    mealybug::run_test("roms/m3_lcdc_tile_sel_change2.gb", "reference/m3_lcdc_tile_sel_change_variant.png", 10);
}
*/

TEST_CASE("mealybug.m3_lcdc_tile_sel_win_change")
{
    mealybug::run_test("roms/m3_lcdc_tile_sel_win_change.gb", "reference/m3_lcdc_tile_sel_win_change.png", 10);
}

// TODO: reference image missing?
/*
TEST_CASE("mealybug.m3_lcdc_tile_sel_win_change2")
{
    mealybug::run_test("roms/m3_lcdc_tile_sel_win_change2.gb", "reference/m3_lcdc_tile_sel_win_change_variant.png", 10);
}
*/

TEST_CASE("mealybug.m3_lcdc_win_en_change_multiple")
{
    mealybug::run_test("roms/m3_lcdc_win_en_change_multiple.gb", "reference/m3_lcdc_win_en_change_multiple.png", 10);
}

TEST_CASE("mealybug.m3_lcdc_win_en_change_multiple_wx")
{
    mealybug::run_test("roms/m3_lcdc_win_en_change_multiple_wx.gb", "reference/m3_lcdc_win_en_change_multiple_wx.png", 10);
}

TEST_CASE("mealybug.m3_lcdc_win_map_change")
{
    mealybug::run_test("roms/m3_lcdc_win_map_change.gb", "reference/m3_lcdc_win_map_change.png", 10);
}

// TODO: reference image missing?
/*
TEST_CASE("mealybug.m3_lcdc_win_map_change2")
{
    mealybug::run_test("roms/m3_lcdc_win_map_change2.gb", "reference/m3_lcdc_win_map_change_variant.png", 10);
}
*/

TEST_CASE("mealybug.m3_obp0_change")
{
    mealybug::run_test("roms/m3_obp0_change.gb", "reference/m3_obp0_change.png", 10);
}

TEST_CASE("mealybug.m3_scx_high_5_bits")
{
    mealybug::run_test("roms/m3_scx_high_5_bits.gb", "reference/m3_scx_high_5_bits.png", 10);
}

// TODO: reference image missing?
/*
TEST_CASE("mealybug.m3_scx_high_5_bits_change2")
{
    mealybug::run_test("roms/m3_scx_high_5_bits_change2.gb", "reference/m3_scx_high_5_bits_change2.png", 10);
}
*/

TEST_CASE("mealybug.m3_scx_low_3_bits")
{
    mealybug::run_test("roms/m3_scx_low_3_bits.gb", "reference/m3_scx_low_3_bits.png", 10);
}

TEST_CASE("mealybug.m3_scy_change")
{
    mealybug::run_test("roms/m3_scy_change.gb", "reference/m3_scy_change.png", 10);
}

// TODO: reference image is missing?
/*
TEST_CASE("mealybug.m3_scy_change2")
{
    mealybug::run_test("roms/m3_scy_change2.gb", "reference/m3_scy_change2.png", 10);
}
*/

TEST_CASE("mealybug.m3_window_timing")
{
    mealybug::run_test("roms/m3_window_timing.gb", "reference/m3_window_timing.png", 10);
}

TEST_CASE("mealybug.m3_window_timing_wx_0")
{
    mealybug::run_test("roms/m3_window_timing_wx_0.gb", "reference/m3_window_timing_wx_0.png", 10);
}

TEST_CASE("mealybug.m3_wx_4_change")
{
    mealybug::run_test("roms/m3_wx_4_change.gb", "reference/m3_wx_4_change.png", 10);
}

TEST_CASE("mealybug.m3_wx_4_change_sprites")
{
    mealybug::run_test("roms/m3_wx_4_change_sprites.gb", "reference/m3_wx_4_change_sprites.png", 10);
}

TEST_CASE("mealybug.m3_wx_5_change")
{
    mealybug::run_test("roms/m3_wx_5_change.gb", "reference/m3_wx_5_change.png", 10);
}

TEST_CASE("mealybug.m3_wx_6_change")
{
    mealybug::run_test("roms/m3_wx_6_change.gb", "reference/m3_wx_6_change.png", 10);
}
