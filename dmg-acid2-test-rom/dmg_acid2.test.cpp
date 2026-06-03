#include "doctest.h"

import mbc;
import graphics;
import utilities;
import cartridge;
import stb_image;
import emulator.core;
import emulator.engine;
import emulator.gameboy;

namespace
{
    template<typename TExpected, typename TError>
    TExpected require_success(std::expected<TExpected, TError>&& result)
    {
        REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
        return std::forward<TExpected>(result.value());
    }
    class test_serial
    {
    public:
        [[nodiscard]] std::uint8_t transfer_bit(const std::uint8_t) { return 0x00; }
    };

    class test_audio_sink
    {
    public:
        static [[nodiscard]] std::uint8_t channel_count() { return 2; }
        static [[nodiscard]] std::uint32_t sample_rate() { return 44100; }
        static void write(const std::span<const float>) {}
    };
}

TEST_CASE("acid.PPU generates output equals to reference image")
{
    test_serial serial {};
    test_audio_sink audio_sink {};

    auto cartridge = require_success(cartridge::load_rom_file("rom/dmg-acid2.gb"));
    auto engine = require_success(emulator::create_engine(cartridge, audio_sink, serial));

    constexpr std::uint8_t num_frames { 10 };

    for (std::uint32_t i = 0; i < num_frames; ++i)
    {
        constexpr std::uint32_t num_ticks_per_frame { 70224 };
        engine->tick(num_ticks_per_frame);
    }

    const std::filesystem::path output_filepath = std::filesystem::temp_directory_path() / "test.png";
    constexpr stb::image_metadata output_metadata
    {
        graphics::lcd_width,
        graphics::lcd_height,
        graphics::num_color_channels
    };

    const auto result = stb::write_png(output_filepath, engine->lcd().data(), output_metadata);
    REQUIRE_MESSAGE(result.has_value(), std::format("Result vram png failed. {}", result.error()));

    std::cout << std::format("Result test image generated at {}", output_filepath.string()) << std:: endl;

    using namespace stb;
    const stb_result<image> reference_img = load_image("reference/reference-dmg.png", graphics::num_color_channels);
    REQUIRE_MESSAGE(reference_img.has_value(), std::format("Could not read reference image. {}", reference_img.error()));

    const auto actual_data = engine->lcd();
    const auto expected_data = reference_img.value().as_span();

    REQUIRE_MESSAGE(std::ranges::equal(actual_data, expected_data), "Result and reference images do not match");
}
