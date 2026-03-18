
export module graphics:registers;

export import memory;
export import utilities;

namespace graphics
{
	export constexpr memory::memory_address_t lcdc_address = 0xFF40;

	export template<memory::ReadOnlyMemory Memory>
	memory::memory_data_t lcdc(const Memory& memory)
	{
		return memory.read(lcdc_address);
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_ppu_enabled(const Memory& memory)
	{
		return utils::is_bit_set<7>(lcdc(memory));
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_window_tilemap_flag_set(const Memory& memory)
	{
		return utils::is_bit_set<6>(lcdc(memory));
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_window_enabled(const Memory& memory)
	{
		return utils::is_bit_set<5>(lcdc(memory));
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_addressing_mode_flag_set(const Memory& memory)
	{
		return utils::is_bit_set<4>(lcdc(memory));
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_background_tilemap_flag_set(const Memory& memory)
	{
		return utils::is_bit_set<3>(lcdc(memory));
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_obj_size_flag_set(const Memory& memory)
	{
		return utils::is_bit_set<2>(lcdc(memory));
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_obj_enabled(const Memory& memory)
	{
		return utils::is_bit_set<1>(lcdc(memory));
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_bg_and_window_display_flag_set(const Memory& memory)
	{
		return utils::is_bit_set<0>(lcdc(memory));
	}

	export constexpr memory::memory_address_t lcd_y_address = 0xFF44;
	export constexpr memory::memory_address_t lcd_cy_address = 0xFF45;

	export template<memory::ReadOnlyMemory Memory>
	memory::memory_data_t lcd_y(const Memory& memory)
	{
		return memory.read(lcd_y_address);
	}

	export constexpr memory::memory_address_t lcd_status_address = 0xFF41;

	export constexpr memory::memory_address_t scy_address = 0xFF42;
	export constexpr memory::memory_address_t scx_address = 0xFF42;

	export constexpr memory::memory_address_t wy_address = 0xFF4A;
	export constexpr memory::memory_address_t wx_address = 0xFF4B;

	export template<memory::ReadOnlyMemory Memory>
	memory::memory_data_t scy(const Memory& memory)
	{
		return memory.read(scy_address);
	}

	export template<memory::ReadOnlyMemory Memory>
	memory::memory_data_t scx(const Memory& memory)
	{
		return memory.read(scx_address);
	}

}
