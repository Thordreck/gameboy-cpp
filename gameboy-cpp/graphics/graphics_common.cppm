export module graphics:common;
export import std;

namespace graphics
{
    export using color_channel_t = std::uint8_t;

    export struct color
    {
        color_channel_t r{};
        color_channel_t g{};
        color_channel_t b{};
        color_channel_t a{};
    };

    export constexpr color white      {255, 255, 255, 255};
    export constexpr color light_gray {170, 170, 170, 255};
    export constexpr color dark_gray  {85,  85,  85,  255};
    export constexpr color black      {0,   0,   0,   255};

    export constexpr color lightest_green   {224, 248, 208, 255};
    export constexpr color light_green      {136, 192, 112, 255};
    export constexpr color dark_green       {52, 104,  86, 255};
    export constexpr color almost_black     {8,   24,  32, 255};

    export using coords_2d_axis_t = std::uint8_t;

    export struct coords_2d
    {
        coords_2d_axis_t x{};
        coords_2d_axis_t y{};
    };

    export using color_index_t = std::uint8_t;
    export using palette_index_t = std::uint8_t;

    export struct pixel
    {
        color_index_t color_index{};
        std::optional<palette_index_t> palette_index{};
        bool background_priority{};

        // For sprite's opaque pixels conflict resolution
        std::optional<std::uint8_t> x {};
        std::optional<std::uint8_t> oam_index {};
    };

    export using lcd_color_palette = std::array<color, 4>;

    export constexpr lcd_color_palette grayscale_lcd_color_palette =
    {
        black, dark_gray, light_gray, white
    };

    export constexpr lcd_color_palette green_lcd_color_palette =
    {
        almost_black, dark_green, light_green, lightest_green
    };

    export bool is_transparent_sprite_pixel(const pixel& pixel)
    {
        return pixel.color_index == 0;
    }

};