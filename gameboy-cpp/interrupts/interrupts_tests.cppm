export module tests;

export import std;
export import memory;
export import interrupts;

namespace tests
{
	export template<interrupts::InterruptDescriptor Interrupt, memory::Memory Memory>
	void execute_complete_dispatch(const Interrupt& interrupt, cpu::cpu_state& cpu, Memory& memory)
	{
		constexpr std::uint8_t num_steps = interrupts::dispatcher::num_steps();
		for (std::uint8_t step = 0; step < num_steps; ++step)
		{
			interrupts::dispatcher::execute(interrupt, cpu, step, memory);
		}
	}

	export class test_memory
	{
	public:
		[[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const { return memory[address]; }
		void write(const memory::memory_address_t address, const memory::memory_data_t value) { memory[address] = value; }

	private:
		std::array<memory::memory_data_t, memory::memory_size> memory {};
	};
}
