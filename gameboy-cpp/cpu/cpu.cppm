
export module cpu;

export import :registers;
export import :memory;
export import :program_counter;

namespace cpu
{
	export struct cpu
	{
		program_counter pc{};
		registers registers{};
		memory memory{};
	};
}
