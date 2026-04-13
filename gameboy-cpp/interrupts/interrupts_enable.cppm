
export module interrupts:enable;
export import memory;
export import :common;

namespace interrupts
{
	export template<InterruptDescriptor Interrupt, memory::Memory Memory>
	void enable(const Interrupt& interrupt, Memory& memory)
	{
		const auto ie_value = memory.read(ie_address);
		memory.write(ie_address, ie_value | interrupt.ie_flag());
	}

	export template<InterruptDescriptor Interrupt, memory::Memory Memory>
	void disable(const Interrupt& interrupt, Memory& memory)
	{
		const auto ie_value = memory.read(ie_address);
		memory.write(ie_address, ie_value & ~interrupt.ie_flag());
	}

	export template<InterruptDescriptor Interrupt, memory::ReadOnlyMemory Memory>
	bool is_enabled(const Interrupt& interrupt, const Memory& memory)
	{
		return (memory.read(ie_address) & interrupt.ie_flag()) == interrupt.ie_flag();
	}
}