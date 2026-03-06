export module cpu:common;

export import memory;
export import :program_counter;
export import :registers;
export import :stack_pointer;
export import :ime;
export import :cycle;
export import :temp_cache;
export import :halt_context;

namespace cpu
{
	export class cpu
	{
	public:
		cpu()
			: pc_{}
			, registers_{ }
			, sp_{ }
			, ime_{}
			, cache_{}
			, halt_{}
			, memory_{ nullptr }
		{}

		program_counter& pc() { return pc_; }
		registers& reg() { return registers_; }
		memory::memory_bus& memory() { return *memory_; }
		stack_pointer& sp() { return sp_; }
		ime& ime_flag() { return ime_; }
		cpu_cycle& cycle() { return cycle_; }
		temp_cache& cache() { return cache_; }
		halt_context& halt_state() { return halt_; }

		const program_counter& pc() const { return pc_; }
		const registers& reg() const { return registers_; }
		const memory::memory_bus& memory() const { return *memory_; }
		const stack_pointer& sp() const { return sp_; }
		const ime& ime_flag() const { return ime_; }
		const cpu_cycle& cycle() const { return cycle_; }
		const temp_cache& cache() const { return cache_; }
		const halt_context& halt_state() const { return halt_; }

		void connect(memory::memory_bus& bus) { memory_ = &bus; }

	private:
		program_counter pc_;
		registers registers_;
		stack_pointer sp_;
		ime ime_;
		cpu_cycle cycle_;
		temp_cache cache_;
		halt_context halt_;

		memory::memory_bus* memory_;
	};
}
