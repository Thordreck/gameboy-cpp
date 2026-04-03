
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

	export template<typename T>
	concept LCD = requires(T& lcd, const coords_2d coords, const color color)
	{
		{ lcd.set_pixel(coords, color) } -> std::same_as<void>;
	};

	using lcd_imp_set_pixel_func_t = void(*)(void*, const coords_2d, const color);

	template<LCD Imp>
	static void set_pixel_imp(void* imp, const coords_2d coords, const color color)
	{
		Imp* lcd = static_cast<Imp*>(imp);
		lcd->set_pixel(coords, color);
	}

	export class lcd
	{
	public:
		template<LCD Imp>
		explicit lcd(Imp& imp)
			: imp{ &imp }
			, set_pixel_imp_fn{ set_pixel_imp<Imp> }
		{}

		void set_pixel(const coords_2d coords, const color color) const
		{
			set_pixel_imp_fn(imp, coords, color);
		}

	private:
		void* imp;
		lcd_imp_set_pixel_func_t set_pixel_imp_fn;
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

	private:
		lcd_raw_memory_t memory;
	};

}
