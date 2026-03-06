export module emulator:cpu_runner;

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

		std::optional<interrupts::interrupt_dispatcher> service_interrupt(cpu::cpu& cpu)
		{
			return cpu.ime_flag().is_enabled()
				? interrupts::service_first_pending_interrupt(cpu)
				: std::nullopt;
		}

	private:
		bool should_enable_ime;
	};

	export template<
		InstructionsProvider instructions_provider,
		InterruptsHandler interrupts_handler>
	class cpu_runner
	{
	public:
		cpu_runner(
			cpu::cpu& cpu, 
			instructions_provider& instructions_provider,
			interrupts_handler& interrupts_handler)
			: cpu{ cpu }
			, instructions { instructions_provider }
			, interrupts { interrupts_handler }
		{}

		void tick()
		{
			handle_tick();
			cpu.cycle()++;
		}

	private:
		void handle_tick()
		{
			// Work is only done at the end of each m cycle
			if (!cpu::is_end_of_any_machine_cycle(cpu.cycle()))
			{
				return;
			}
			
			// Halt 
			if (cpu.halt_state().enabled)
			{
				if (!interrupts::is_any_interrupt_pending(cpu))
				{
					return;
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
					cpu.cycle() = {};
				}

				return;
			}

			// Fetch
			if (!active_instruction.has_value())
			{
				fetch_decode_opcode();
				cpu.cycle() = {};

				return;
			}

			// Execution
			const opcodes::instruction instruction = active_instruction.value();
			instruction.execute(cpu);

			if (!opcodes::is_instruction_done(cpu, instruction))
			{
				return;
			}

			active_instruction.reset();
			cpu.cycle() = {};

			// Halt bug
			if (cpu.halt_state().enabled
				&& !cpu.halt_state().ime_flag_set
				&& cpu.halt_state().interrupts_pending)
			{
				const opcodes::opcode_t next_opcode = cpu.memory().read(cpu.pc());
				active_instruction = instructions.get(next_opcode);
				cpu.halt_state() = {};

				return;
			}

			interrupts.enable_ime_if_requested(cpu);
			active_interrupt = interrupts.service_interrupt(cpu);

			if (!active_interrupt.has_value())
			{
				fetch_decode_opcode();
			}
		}

		void fetch_decode_opcode()
		{
			const opcodes::opcode_t next_opcode = cpu.memory().read(cpu.pc()++);
			active_instruction = instructions.get(next_opcode);
		}

	private:
		cpu::cpu& cpu;

		std::optional<interrupts::interrupt_dispatcher> active_interrupt;
		std::optional<opcodes::instruction> active_instruction;

		instructions_provider& instructions;
		interrupts_handler& interrupts;
	};

	export using default_cpu_runner = cpu_runner<default_instructions_provider, default_interrupt_handler>;
}