export module tests.core:graphics;

import std;
import graphics;
import stb_image;
import utilities;

import :common;

namespace tests
{
    export struct dummy_lcd
    {
        static void write_frame(const graphics::lcd_memory_view_t) {}
    };

    export class memory_lcd
    {
    public:
        void write_frame(const graphics::lcd_memory_view_t new_frame)
        {
            std::ranges::copy(new_frame.begin(), new_frame.end(), frame.begin());
        }

        [[nodiscard]] std::span<const std::uint8_t> last_frame() const { return frame; }

    private:
        graphics::lcd_memory_t frame {};
    };

    export [[nodiscard]] outcome save_lcd_screenshot(
        const std::span<const std::uint8_t> lcd,
        const std::filesystem::path& path)
    {
        using namespace stb;
        using namespace graphics;

        if (lcd.size() != lcd_memory_size)
        {
            return std::unexpected {std::format("Invalid lcd size. Expected: {}. Actual: {}", lcd_memory_size, lcd.size()) };
        }

        constexpr image_metadata output_metadata { lcd_width, lcd_height, num_color_channels };
        return write_png(path, lcd.data(), output_metadata);
    }

    export [[nodiscard]] result<std::filesystem::path> save_lcd_screenshot(const std::span<const std::uint8_t> lcd)
    {
        using namespace utils;
        namespace files = std::filesystem;

        const files::path output_filepath = files::temp_directory_path() / (generate_uuid() + ".png");

        return save_lcd_screenshot(lcd, output_filepath)
            .transform([&output_filepath] { return output_filepath; });
    }

    export [[nodiscard]] result<bool> compare_lcd_with_reference(
        const std::span<const std::uint8_t> lcd,
        const std::filesystem::path& reference_path)
    {
        using namespace stb;
        using namespace graphics;

        return load_image(reference_path, num_color_channels)
            .transform([lcd] (const auto& reference) { return std::ranges::equal(reference.as_span(), lcd); });
    }

}