
export module opcodes:fetch;

import cpu;
import std;

namespace opcodes
{
	export template<typename T>
	concept OpcodeFetcher = requires(const cpu::cpu & cpu)
	{
		{ T::fetch(cpu) } -> std::convertible_to<std::uint8_t>;
	};

	export struct default_opcode_fetcher
	{
		static std::uint8_t fetch(const cpu::cpu& cpu)
		{
			return cpu.memory()[cpu.pc()];
		}
	};
}
