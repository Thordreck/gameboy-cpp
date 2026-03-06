
export module interrupts:request;

export import std;
export import memory;
export import :common;

namespace interrupts
{
	export template<InterruptDescriptor interrupt>
	void request(memory::memory_bus& memory)
	{
		const auto value = memory.read(if_address);
		memory.write(if_address, value | interrupt::if_flag);
	}

	export template<InterruptDescriptor interrupt>
	void clear_request(memory::memory_bus& memory)
	{
		const auto value = memory.read(if_address);
		memory.write(if_address, value & ~interrupt::if_flag);
	}

	export template<InterruptDescriptor interrupt>
	bool is_requested(const memory::memory_bus& memory)
	{
		return (memory.read(if_address) & interrupt::if_flag) == interrupt::if_flag;
	}
}
