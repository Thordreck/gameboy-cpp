export module cpu:common;

export import :program_counter;
export import :registers;
export import :stack_pointer;
export import :ime;
export import :temp_cache;
export import :halt_context;

namespace cpu
{
	export struct cpu_state
	{
		program_counter pc {};
		registers reg {};
		stack_pointer sp {};
		ime_state ime {};
		temp_cache cache {};
		halt_context halt {};
	};
}
