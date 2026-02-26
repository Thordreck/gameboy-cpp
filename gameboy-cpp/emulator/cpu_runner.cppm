export module emulator:cpu_runner;

export import cpu;

import std;
import opcodes;

namespace emulator
{
	export class cpu_runner
	{
	private:
		enum class state : std::uint8_t
		{
			fetch,
			decode,
			execute,
			interrupt,
			halt
		};

	public:
		cpu_runner(cpu::cpu& cpu)
			: cpu{ cpu }
			, state{ state::fetch }
		{}

		// TODO: rethink this. Possibly make handlers configurable.
		void tick()
		{
			cpu.cycle()++;

			if (!cpu::is_end_of_any_machine_cycle(cpu.cycle()))
			{
				return;
			}
			
			switch (state)
			{
			case state::fetch: 
				handle_fetch();
				break;
			case state::decode: 
				handle_decode();
				break;
			case state::execute: 
				handle_execute();
				break;
			case state::interrupt: 
				handle_interrupt();
				break;
			case state::halt: 
				handle_halt();
				break;
			}
		}

	private:
		void handle_fetch()
		{
			current_opcode = cpu.memory()[cpu.pc()++];

			state = state::decode;
			cpu.cycle().m_cycle() = {};
		}

		void handle_decode()
		{
		}

		void handle_execute()
		{
		}

		void handle_interrupt()
		{
		}

		void handle_halt()
		{
		}

	private:
		cpu::cpu& cpu;
		state state;

		std::optional<opcodes::opcode_t> current_opcode;
		std::optional<opcodes::instruction> current_instruction;
		std::optional<opcodes::instruction_table&> active_table;
	};
}