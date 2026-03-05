export module tests;

export import std;
export import memory;
export import interrupts;

namespace tests
{
	export template<interrupts::InterruptDescriptor interrupt, std::uint16_t handler_address, std::uint8_t if_flag, std::uint8_t ie_flag>
	requires interrupts::InterruptAddress<handler_address> && interrupts::InterruptFlag<if_flag> && interrupts::InterruptFlag<ie_flag>
	struct interrupt_test_case
	{
		using interrupt_t = interrupt;
		static constexpr auto handler = handler_address;
		static constexpr auto if_flag = if_flag;
		static constexpr auto ie_flag = ie_flag;
	};

	export using vblank_interrupt_test_case = interrupt_test_case<interrupts::vblank_interrupt, 0x40, 0x1, 0x1>;
	export using lcd_interrupt_test_case = interrupt_test_case<interrupts::lcd_interrupt, 0x48, 0x2, 0x2>;
	export using timer_interrupt_test_case = interrupt_test_case<interrupts::timer_interrupt, 0x50, 0x4, 0x4>;
	export using serial_interrupt_test_case = interrupt_test_case<interrupts::serial_interrupt, 0x58, 0x8, 0x8>;
	export using joypad_interrupt_test_case = interrupt_test_case<interrupts::joypad_interrupt, 0x60, 0x10, 0x10>;

	export template<interrupts::InterruptDispatcher interrupt_dispatcher>
	void execute_complete_dispatch(interrupt_dispatcher dispatcher, cpu::cpu& cpu)
	{
		cpu.cycle() = {};

		while (cpu.cycle().m_cycle() < dispatcher.num_cycles())
		{
			dispatcher.execute(cpu);
			cpu.cycle()++;
		}
	}

	export template<interrupts::InterruptDescriptor interrupt>
	void execute_complete_dispatch(cpu::cpu& cpu)
	{
		using dispatcher_t = typename interrupts::dispatcher<interrupt>;
		execute_complete_dispatch<dispatcher_t>({}, cpu);
	}

	export class mock_memory_bus
	{
	public:
		mock_memory_bus()
			: memory {}
			, memory_region{ memory::map(memory) }
			, memory_map{ memory::build_memory_map(memory_region) }
			, memory_bus{ memory_map }
		{}

		memory::memory_bus& bus() { return memory_bus; }

	private:
		std::array<memory::memory_data_t, memory::memory_size> memory;
		memory::span_map<memory::memory_size> memory_region;
		memory::memory_map_array_t memory_map;
		memory::memory_bus memory_bus;

	};
}
