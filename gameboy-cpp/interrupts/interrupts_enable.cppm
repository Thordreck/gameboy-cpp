
export module interrupts:enable;
export import memory;
export import :common;

namespace interrupts
{
	export template<InterruptDescriptor interrupt>
	void enable(memory::memory_bus& memory)
	{
		const auto ie_value = memory.read(ie_address);
		memory.write(ie_address, ie_value | interrupt::ie_flag);
	}

	export template<InterruptDescriptor interrupt>
	void disable(memory::memory_bus& memory)
	{
		const auto ie_value = memory.read(ie_address);
		memory.write(ie_address, ie_value & ~interrupt::ie_flag);
	}

	export template<InterruptDescriptor interrupt>
	bool is_enabled(const memory::memory_bus& memory)
	{
		return (memory.read(ie_address) & interrupt::ie_flag) == interrupt::ie_flag;
	}
}