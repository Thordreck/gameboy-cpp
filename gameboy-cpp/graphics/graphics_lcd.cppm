
export module graphics:lcd;

export import std;
export import :common;

namespace graphics
{
	export constexpr std::uint8_t lcd_width = 160;
	export constexpr std::uint8_t lcd_height = 144;
	export constexpr std::size_t lcd_num_pixels = lcd_width * lcd_height;
	export constexpr std::size_t lcd_memory_size = lcd_num_pixels * num_color_channels;

	export using lcd_raw_memory_t = std::span<std::uint8_t, lcd_memory_size>;
	export using lcd_scanline_t = std::span<const std::uint8_t, lcd_width * num_color_channels>;

	export template<typename T>
	concept LCD = requires(T& lcd, const coords_2d coords, const color color, const std::uint8_t scanline_idx, const lcd_scanline_t scanline)
	{
		{ lcd.set_pixel(coords, color) } -> std::same_as<void>;
		{ lcd.set_scanline(scanline_idx, scanline) } -> std::same_as<void>;
	};

	export class raw_memory_lcd
	{
	public:
		explicit raw_memory_lcd(const lcd_raw_memory_t memory)
			: memory { memory }
		{}

		void set_pixel(const coords_2d coords, const color color) const
		{
			const std::size_t pixel_data_pos = (coords.y * lcd_width + coords.x) * num_color_channels;

			memory[pixel_data_pos] = color.r;
			memory[pixel_data_pos + 1] = color.g;
			memory[pixel_data_pos + 2] = color.b;
		}

		void set_scanline(const std::uint8_t scanline_idx, const lcd_scanline_t scanline) const
		{
			const std::size_t pixel_data_pos = scanline_idx * lcd_width * num_color_channels;
			std::ranges::copy(scanline, memory.begin() + pixel_data_pos);
		}

	private:
		lcd_raw_memory_t memory;
	};

}
