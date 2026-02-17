
export module cpu;

export import :registers;
export import :program_counter;
export import :stack_pointer;
export import :memory_bus;

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
		{}

		program_counter& pc() { return pc_; }
		registers& reg() { return registers_; }
		memory_bus& memory() { return memory_; }
		stack_pointer& sp() { return sp_; }

		const program_counter& pc() const { return pc_; }
		const registers& reg() const { return registers_; }
		const memory_bus& memory() const { return memory_; }
		const stack_pointer& sp() const { return sp_; }

	private:
		program_counter pc_;
		registers registers_;
		memory_bus memory_;
		stack_pointer sp_;
	};
}
