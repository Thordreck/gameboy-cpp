#include "doctest.h"

import engine;
import cartridge;
import tests.core;

TEST_CASE("acid.PPU generates output equals to reference image")
{
    using namespace tests;
    using namespace engine;

    memory_lcd lcd {};
    dummy_serial serial {};
    dummy_audio audio {};

    auto cartridge = required(cartridge::load_rom_file("rom/dmg-acid2.gb"));
    auto engine = required(create_engine(cartridge, lcd, audio, serial));

    constexpr std::uint8_t num_frames { 10 };
    tick_frames(*engine, num_frames);

    constexpr char const* ref_img_path { "reference/reference-dmg.png" };
    const bool generated_expected_output = required(compare_lcd_with_reference(lcd.last_frame(), ref_img_path));

    if (!generated_expected_output)
    {
        const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));
        FAIL(std::format("Incorrect lcd result generated. Generated result image at {}", image_path.string()));
    }
}
