export module tests;

import std;

import cpu;
import memory;
import interrupts;

namespace tests
{
	export template<memory::Memory Memory, interrupts::InterruptController InterruptController>
	void execute_complete_dispatch(
		const interrupts::interrupt interrupt,
		cpu::cpu_state& cpu,
		Memory& memory,
		InterruptController& interrupts)
	{
		using namespace interrupts;
		constexpr std::uint8_t num_steps = interrupt_dispatcher::num_steps();

		for (std::uint8_t step = 0; step < num_steps; ++step)
		{
			interrupt_dispatcher::execute(interrupt, cpu, step, memory, interrupts);
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
