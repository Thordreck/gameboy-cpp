export module emulator:memory_map;

export import std;
export import timer;
export import memory;
export import graphics;
export import utilities;
export import interrupts;

namespace emulator
{
	export class vram_memory_page
	{
	public:
		static constexpr memory::memory_address_t start = graphics::vram_start_address;
		static constexpr memory::memory_address_t end = graphics::vram_end_address;

		explicit vram_memory_page(
			const graphics::pixel_processing_unit& ppu,
			graphics::vram_t vram)
			: ppu { ppu }
			, vram { vram }
		{}

		[[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const
		{
			return ppu.mode() == graphics::ppu_mode::drawing
				? 0xFF
				: graphics::read_vram(vram, address);
		}

		void write(const memory::memory_address_t address, const memory::memory_data_t value)
		{
			if (ppu.mode() != graphics::ppu_mode::drawing)
			{
				graphics::write_vram(vram, address, value);
			}
		}

	private:
		const graphics::pixel_processing_unit& ppu;
		graphics::vram_t vram;
	};

	export class io_hram_interrupt_memory_page
	{
	public:
		static constexpr memory::memory_address_t start = 0xFF00;
		static constexpr memory::memory_address_t end = 0xFFFF;

		io_hram_interrupt_memory_page(
			timer::timer_system& timers,
			interrupts::interrupt_registers& interrupts,
			graphics::pixel_processing_unit& ppu)
			: timers{ timers }
			, interrupts{ interrupts }
			, ppu { ppu }
		{}

		[[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const
		{
			switch (address)
			{
			case timer::div_address: return static_cast<memory::memory_data_t>(timers.divider() >> 8);
			case timer::tima_address: return timers.counter();
			case timer::tma_address: return timers.modulo().value;
			case timer::tac_address: return timers.control();
			case graphics::lcd_status_address: return graphics::lcd_status(ppu);
			case graphics::lcdc_address: return (ppu.is_enabled() << 7) | fallback_memory[address - start];
			case graphics::lcd_y_address: return ppu.scanline();
			case graphics::lcd_cy_address: return ppu.lyc();
			case interrupts::if_address: return interrupts.flag;
			case interrupts::ie_address: return interrupts.enable;
			default: return fallback_memory[address - start];
			}
		}

		void write(const memory::memory_address_t address, const memory::memory_data_t value)
		{
			switch (address)
			{
			case timer::div_address: 
				timers.divider() = 0x00;
				break;
			case timer::tima_address:
				timers.counter() = value;
				timers.cancel_pending_interrupt();
				break;
			case timer::tma_address: 
				timers.modulo().value = value;
				break;
			case timer::tac_address: 
				timers.control() = value;
				break;
			case graphics::lcd_status_address:
				graphics::set_lcd_status(value, ppu);
				break;
			case graphics::lcdc_address:
				ppu.set_enabled(utils::is_bit_set<7>(value));
				fallback_memory[address - start] = value;
				break;
			case graphics::lcd_y_address: // readonly
				break;
			case graphics::lcd_cy_address:
				ppu.set_lyc(value);
				break;
			case interrupts::if_address:
				interrupts.flag = value;
				break;
			case interrupts::ie_address: 
				interrupts.enable = value;
				break;
			default: 
				fallback_memory[address - start] = value;
			}
		}

	private:
		timer::timer_system& timers;
		interrupts::interrupt_registers& interrupts;
		graphics::pixel_processing_unit& ppu;

		// TODO: replace by proper mapped memory
		std::array<memory::memory_data_t, end - start + 1> fallback_memory {};
	};
}