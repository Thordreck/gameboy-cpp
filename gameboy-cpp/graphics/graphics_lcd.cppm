
export module graphics:lcd;

export import std;
export import :common;

namespace graphics
{
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
}
