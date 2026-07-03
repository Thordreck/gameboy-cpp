module;
#include "doctest.h"

export module scribbl;

import std;
import mbc;
import memory;
import graphics;
import utilities;
import cartridge;
import stb_image;
import emulator.core;
import emulator.engine;
import emulator.gameboy;

namespace scribbl
{
    template<typename TExpected, typename TError>
    TExpected require_success(std::expected<TExpected, TError>&& result)
    {
        REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
        return std::forward<TExpected>(result.value());
    }

    class test_audio_sink
    {
    public:
        static [[nodiscard]] std::uint8_t channel_count() { return 2; }
        static [[nodiscard]] std::uint32_t sample_rate() { return 44100; }
        static void write(const std::span<const float>) {}
    };

    class test_serial
    {
    public:
        [[nodiscard]] std::uint8_t transfer_bit(const std::uint8_t bit) { return 0x00; }
    };

    template<emulator::Engine Engine>
    [[nodiscard]] std::expected<std::filesystem::path, std::string> export_lcd(
        const Engine& engine,
        const std::string_view rom_file_path)
    {
        const std::filesystem::path output_filepath
            = std::filesystem::temp_directory_path()
            / std::filesystem::path(rom_file_path).filename().replace_extension("png");

        constexpr stb::image_metadata output_metadata
        {
            graphics::lcd_width,
            graphics::lcd_height,
            graphics::num_color_channels
        };

        const auto result = stb::write_png(output_filepath, engine.lcd().data(), output_metadata);
        return result.transform([output_filepath] { return output_filepath; });
    }

    export void run_lcd_test(
        const std::string_view rom_file_path,
        const std::string_view expected_output_path,
        const size_t num_frames)
    {
        test_serial serial {};
        test_audio_sink audio_sink {};

        auto cartridge = require_success(cartridge::load_rom_file(rom_file_path));
        auto engine = require_success(emulator::create_engine(cartridge, audio_sink, serial));

        for (size_t i = 0; i < num_frames; i++)
        {
            constexpr std::uint32_t num_ticks_per_frame { 70224 };
            engine->tick(num_ticks_per_frame);
        }

        using namespace stb;
        const stb_result<image> reference_img = load_image(expected_output_path, graphics::num_color_channels);
        REQUIRE_MESSAGE(reference_img.has_value(), std::format("Could not read reference image. {}", reference_img.error()));

        const auto actual_data = engine->lcd();
        const auto expected_data = reference_img.value().as_span();
        const bool generated_expected_output = std::ranges::equal(expected_data, actual_data);

        if (!generated_expected_output)
        {
            const auto image_path = require_success(export_lcd(*engine, rom_file_path));
            FAIL(std::format("Incorrect lcd result generated. Generated result image at {}", image_path.string()));
        }
    }

}