export module interrupts:service;

export import std;
export import cpu;

export import :common;
export import :enable;
export import :request;
export import :dispatch;

namespace interrupts
{
	export template <InterruptDescriptor Interrupt, memory::ReadOnlyMemory Memory>
	bool is_pending(const Interrupt& interrupt, const Memory& memory)
	{
		return is_enabled(interrupt, memory) && is_requested(interrupt, memory);
	}

	export template<memory::ReadOnlyMemory Memory>
	bool is_any_interrupt_pending(const Memory& memory)
	{
		return (memory.read(ie_address) & ie_mask) & (memory.read(if_address) & if_mask);
	}

	export template<memory::ReadOnlyMemory Memory>
	std::optional<interrupt> get_first_pending_interrupt(const Memory& memory)
	{
		constexpr std::array interrupts_by_priority
		{
			vblank_interrupt,
			lcd_interrupt,
			timer_interrupt,
			serial_interrupt,
			joypad_interrupt
		};

		for (const auto& interrupt : interrupts_by_priority)
		{
			if (is_pending(interrupt, memory))
			{
				return interrupt;
			}
		}

		return std::nullopt;
	}
}
