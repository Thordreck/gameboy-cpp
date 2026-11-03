
export module graphics:registers;

import memory;
import utilities;

namespace graphics
{
	export constexpr memory::memory_address_t lcdc_address = 0xFF40;
	export constexpr memory::memory_address_t lcd_status_address = 0xFF41;
	export constexpr memory::memory_address_t scy_address = 0xFF42;
	export constexpr memory::memory_address_t scx_address = 0xFF43;
	export constexpr memory::memory_address_t lcd_y_address = 0xFF44;
	export constexpr memory::memory_address_t lcd_cy_address = 0xFF45;
	export constexpr memory::memory_address_t bgp_address = 0xFF47;
	export constexpr memory::memory_address_t obp0_address = 0xFF48;
	export constexpr memory::memory_address_t obp1_address = 0xFF49;
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

	export template<memory::ReadOnlyMemory Memory>
	memory::memory_data_t wy(const Memory& memory)
	{
		return memory.read(wy_address);
	}

	export template<memory::ReadOnlyMemory Memory>
	memory::memory_data_t wx(const Memory& memory)
	{
		return memory.read(wx_address);
	}

}
