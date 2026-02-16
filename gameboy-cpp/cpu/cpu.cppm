
export module cpu;

export import :registers;
export import :memory;
export import :program_counter;
export import :stack_pointer;

namespace cpu
{
	export struct cpu
	{
		program_counter pc{};
		registers registers{};
		memory memory{};
		stack_pointer sp{};
	};
}
