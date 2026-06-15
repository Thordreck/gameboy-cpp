
export module cpu:ime;

import std;

namespace cpu
{
	export struct ime_state
	{
		bool enabled { false };
		std::uint32_t ticks_until_enabled {};
	};
}
