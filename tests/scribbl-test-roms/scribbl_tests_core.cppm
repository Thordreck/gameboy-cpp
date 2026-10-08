module;
#include "doctest.h"

export module scribbl;

import std;
import engine;
import cartridge;
import tests.core;

namespace scribbl
{
    export void run_lcd_test(
        const std::string_view rom_file_path,
        const std::string_view expected_output_path,
        const size_t num_frames)
    {
        using namespace tests;
        using namespace engine;
        using namespace cartridge;

        memory_lcd lcd {};
        dummy_serial serial {};
        dummy_audio audio_sink {};

        auto cartridge = required(load_rom_file(rom_file_path));
        auto engine = required(create_engine(cartridge, lcd, audio_sink, serial));

        tick_frames(*engine, num_frames);
        const bool generated_expected_output = required(compare_lcd_with_reference(lcd.last_frame(), expected_output_path));

        if (!generated_expected_output)
        {
            const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));
            FAIL(std::format("Incorrect lcd result generated. Generated result image at {}", image_path.string()));
        }
    }

}