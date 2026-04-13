
export module interrupts:request;

export import std;
export import memory;
export import :common;

namespace interrupts
{
	export template<InterruptDescriptor Interrupt, memory::Memory Memory>
	void request(const Interrupt& interrupt, Memory& memory)
	{
		const auto value = memory.read(if_address);
		memory.write(if_address, value | interrupt.if_flag());
	}

	export template<InterruptDescriptor Interrupt, memory::Memory Memory>
	void clear_request(const Interrupt& interrupt, Memory& memory)
	{
		const auto value = memory.read(if_address);
		memory.write(if_address, value & ~interrupt.if_flag());
	}

	export template<InterruptDescriptor Interrupt, memory::ReadOnlyMemory Memory>
	bool is_requested(const Interrupt& interrupt, const Memory& memory)
	{
		return (memory.read(if_address) & interrupt.if_flag()) == interrupt.if_flag();
	}
}
