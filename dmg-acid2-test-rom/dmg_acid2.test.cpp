#include "doctest.h"

import emulator.core;
import utilities;
import stb_image;

namespace
{
    template<typename TExpected, typename TError>
    TExpected require_success(const std::expected<TExpected, TError>& result)
    {
        REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
        return result.value();
    }
}

TEST_CASE("acid.PPU generates output equals to reference image")
{
    // engine
    emulator::engine engine {};

    // rom
    {
        const auto rom_data = require_success(utils::read_binary_file("dmg-acid2.gb"));
        engine.load_rom(rom_data);
    }

    constexpr std::uint8_t num_frames { 10 };

    for (std::uint32_t i = 0; i < num_frames; ++i)
    {
        constexpr std::uint32_t num_ticks_per_frame { 70224 };
        engine.tick(num_ticks_per_frame);
    }

    const std::filesystem::path output_filepath = std::filesystem::temp_directory_path() / "test.png";
    constexpr stb::image_metadata output_metadata
    {
        graphics::lcd_width,
        graphics::lcd_height,
        graphics::num_color_channels
    };

    const auto result = stb::write_png(output_filepath, engine.lcd().data(), output_metadata);
    REQUIRE_MESSAGE(result.has_value(), std::format("Result vram png failed. {}", result.error()));

    std::cout << std::format("Result test image generated at {}", output_filepath.string()) << std:: endl;

    using namespace stb;
    const stb_result<image> reference_img = load_image("reference-dmg.png", graphics::num_color_channels);
    REQUIRE_MESSAGE(reference_img.has_value(), std::format("Could not read reference image. {}", reference_img.error()));

    const auto actual_data = engine.lcd();
    const auto expected_data = reference_img.value().as_span();

    REQUIRE_MESSAGE(std::ranges::equal(actual_data, expected_data), "Result and reference images do not match");
}
