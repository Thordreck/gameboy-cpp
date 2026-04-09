module;
#include "profiling.hpp"

export module emulator.core:cpu;

export import std;
export import cpu;
export import opcodes;
export import interrupts;

namespace emulator
{
	export template<typename T>
	concept InstructionsProvider = requires(
		T& provider, 
		const opcodes::opcode_t opcode)
	{
		{ provider.get(opcode) } -> std::convertible_to<std::optional<opcodes::instruction>>;
	};

	export class default_instructions_provider
	{
	public:
		default_instructions_provider()
			: prefix_mode{ false }
		{}

		std::optional<opcodes::instruction> get(const opcodes::opcode_t opcode)
		{
			if (!prefix_mode && opcode == opcodes::prefix_opcode)
			{
				prefix_mode = true;
				return std::nullopt;
			}

			opcodes::instruction instruction = prefix_mode
				? opcodes::default_prefixed_instruction_table[opcode]
				: opcodes::default_instruction_table[opcode];

			prefix_mode = false;
			return instruction;
		}

	private:
		bool prefix_mode;
	};

	export template<typename T>
	concept InterruptsHandler = requires(
		T& handler, 
		cpu::cpu& cpu)
	{
		{ handler.enable_ime_if_requested(cpu) } -> std::same_as<void>;
		{ handler.service_interrupt(cpu) } -> std::convertible_to<std::optional<interrupts::interrupt_dispatcher>>;
	};

	export class default_interrupt_handler
	{
	public:
		default_interrupt_handler()
			: should_enable_ime{ false  }
		{}

		void enable_ime_if_requested(cpu::cpu& cpu)
		{
			if (!cpu.ime_flag().is_requested())
			{
				return;
			}

			if (should_enable_ime)
			{
				cpu.ime_flag().enable();
			}

			should_enable_ime = !should_enable_ime;
		}

		static std::optional<interrupts::interrupt_dispatcher> service_interrupt(cpu::cpu& cpu)
		{
			return cpu.ime_flag().is_enabled()
				? interrupts::service_first_pending_interrupt(cpu)
				: std::nullopt;
		}

	private:
		bool should_enable_ime;
	};

	export template<InstructionsProvider instructions_provider, InterruptsHandler interrupts_handler>
	requires std::is_default_constructible_v<instructions_provider> && std::is_default_constructible_v<interrupts_handler>
	class cpu_runner
	{
	public:
		explicit cpu_runner(cpu::cpu& cpu)
			: cpu{ cpu }
			, instructions { }
			, interrupts { }
		{}

		void tick(const std::uint32_t num_ticks)
		{
			PROFILER_SCOPE("CPU Runner:tick()");
			std::uint32_t remaining_ticks = num_ticks;

			while (remaining_ticks-- > 0)
			{
				// Work is only done at the end of each m cycle
				if (!cpu::is_end_of_any_machine_cycle(cpu.cycle()))
				{
					++cpu.cycle();
				}
				else if (const bool should_reset_m_cycle = handle_end_m_cycle())
				{
					cpu.cycle() = {};
				}
				else
				{
					++cpu.cycle();
				}
			}
		}
		[[nodiscard]] std::uint32_t tick_batch() const
		{
			return active_interrupt
				.transform([](const auto& interrupt) { return interrupt.num_cycles(); })
				.value_or(
					active_instruction
						.transform([](const auto& interrupt) { return interrupt.num_cycles(); })
						.value_or(4));
		}

	private:
		bool handle_end_m_cycle()
		{
			// Halt
			if (cpu.halt_state().enabled)
			{
				if (!interrupts::is_any_interrupt_pending(cpu))
				{
					return true;
				}

				active_interrupt = cpu.halt_state().ime_flag_set
					? interrupts.service_interrupt(cpu)
					: std::nullopt;

				cpu.halt_state() = {};
			}
			
			// Interrupts
			if (active_interrupt.has_value())
			{
				const interrupts::interrupt_dispatcher dispatcher = active_interrupt.value();
				dispatcher.execute(cpu);

				if (interrupts::is_interrupt_dispatched(cpu, dispatcher))
				{
					active_interrupt.reset();
					return true;
				}

				return false;
			}

			// Fetch
			if (!active_instruction.has_value())
			{
				fetch_decode_opcode();
				return true;
			}

			// Execution
			const opcodes::instruction instruction = active_instruction.value();
			instruction.execute(cpu);

			if (!opcodes::is_instruction_done(cpu, instruction))
			{
				return false;
			}

			active_instruction.reset();

			// Halt bug
			if (cpu.halt_state().enabled
				&& !cpu.halt_state().ime_flag_set
				&& cpu.halt_state().interrupts_pending)
			{
				const opcodes::opcode_t next_opcode = cpu.memory().read(cpu.pc());
				active_instruction = instructions.get(next_opcode);
				cpu.halt_state() = {};

				return true;
			}

			interrupts.enable_ime_if_requested(cpu);
			active_interrupt = cpu.ime_flag().is_enabled()
				? interrupts.service_interrupt(cpu)
				: std::nullopt;

			if (!active_interrupt.has_value())
			{
				fetch_decode_opcode();
			}

			return true;
		}

		void fetch_decode_opcode()
		{
			const opcodes::opcode_t next_opcode = cpu.memory().read(cpu.pc()++);
			active_instruction = instructions.get(next_opcode);
		}

	private:
		cpu::cpu& cpu;

		std::optional<interrupts::interrupt_dispatcher> active_interrupt {};
		std::optional<opcodes::instruction> active_instruction {};

		instructions_provider instructions;
		interrupts_handler interrupts;
	};

	export using default_cpu_runner = cpu_runner<default_instructions_provider, default_interrupt_handler>;

}