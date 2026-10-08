#include "doctest.h"

import std;
import engine;
import cartridge;
import tests.core;

TEST_CASE("mooneye.manual_only.sprite_priority")
{
    using namespace tests;
    using namespace engine;
    using namespace cartridge;

    memory_lcd lcd {};
    dummy_audio audio {};
    dummy_serial serial {};

    auto cartridge = required(load_rom_file("roms/manual-only/roms/sprite_priority.gb"));
    auto engine = required(create_engine(cartridge, lcd, audio, serial));

    constexpr std::uint8_t num_frames { 10 };
    tick_frames(*engine, num_frames);

    const bool generated_expected_output = required(compare_lcd_with_reference(
        lcd.last_frame(),
        "roms/manual-only/reference/sprite_priority-expected.png"));

    if (!generated_expected_output)
    {
        const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));
        FAIL(std::format("Incorrect lcd result generated. Generated result image at {}", image_path.string()));
    }
}
