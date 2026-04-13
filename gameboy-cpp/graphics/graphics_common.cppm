export module graphics:common;
export import std;

namespace graphics
{
    export using color_channel_t = std::uint8_t;
    export constexpr std::uint8_t num_color_channels = 3;

    export struct color
    {
        color_channel_t r{};
        color_channel_t g{};
        color_channel_t b{};
    };

    export constexpr color white      {255, 255, 255 };
    export constexpr color light_gray {170, 170, 170 };
    export constexpr color dark_gray  {85,  85,  85  };
    export constexpr color black      {0,   0,   0   };

    export constexpr color lightest_green   {224, 248, 208 };
    export constexpr color light_green      {136, 192, 112 };
    export constexpr color dark_green       {52, 104,  86  };
    export constexpr color almost_black     {8,   24,  32  };

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

    export constexpr std::uint8_t palette_num_colors = 4;
    export using lcd_color_palette_t = std::span<const color, palette_num_colors>;

    export constexpr std::array grayscale_lcd_color_palette =
    {
        white, light_gray, dark_gray, black
    };

    export constexpr std::array green_lcd_color_palette =
    {
        almost_black, dark_green, light_green, lightest_green
    };

    export bool is_transparent_sprite_pixel(const pixel& pixel)
    {
        return pixel.color_index == 0;
    }

};