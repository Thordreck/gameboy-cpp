export module cpu:common;

import :memory_bus;
import :program_counter;
import :registers;
import :stack_pointer;
import :ime;
import :cycle;
import :temp_cache;

namespace cpu
{
	export class cpu
	{
	public:
		cpu(memory_bus::data_t memory_data)
			: pc_{}
			, memory_{ memory_data }
			, registers_{ }
			, sp_{ }
			, ime_{}
			, cache_{}
		{}

		program_counter& pc() { return pc_; }
		registers& reg() { return registers_; }
		memory_bus& memory() { return memory_; }
		stack_pointer& sp() { return sp_; }
		ime& ime_flag() { return ime_; }
		cpu_cycle& cycle() { return cycle_; }
		temp_cache& cache() { return cache_; }

		const program_counter& pc() const { return pc_; }
		const registers& reg() const { return registers_; }
		const memory_bus& memory() const { return memory_; }
		const stack_pointer& sp() const { return sp_; }
		const ime& ime_flag() const { return ime_; }
		const cpu_cycle& cycle() const { return cycle_; }
		const temp_cache& cache() const { return cache_; }

	private:
		program_counter pc_;
		registers registers_;
		memory_bus memory_;
		stack_pointer sp_;
		ime ime_;
		cpu_cycle cycle_;
		temp_cache cache_;
	};
}
