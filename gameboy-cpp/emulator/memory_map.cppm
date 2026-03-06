export module emulator:memory_map;

export import memory;
export import timer;
export import interrupts;
export import std;
export import utilities;

namespace emulator
{
	export class io_hram_interrupt_memory_page
	{
	public:
		static constexpr memory::memory_address_t start = 0xFF00;
		static constexpr memory::memory_address_t end = 0xFFFF;

		io_hram_interrupt_memory_page(
			timer::timer_system& timers,
			interrupts::interrupt_registers& interrupts)
			: timers{ timers }
			, interrupts{ interrupts }
			, fallback_memory{ }
		{}

		memory::memory_data_t read(const memory::memory_address_t address) const
		{
			switch (address)
			{
			case timer::div_address: return static_cast<memory::memory_data_t>(timers.divider() >> 8);
			case timer::tima_address: return timers.counter();
			case timer::tma_address: return timers.modulo().value;
			case timer::tac_address: return timers.control();
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

		// TODO: replace by proper mapped memory
		std::array<memory::memory_data_t, end - start + 1> fallback_memory;
	};
}