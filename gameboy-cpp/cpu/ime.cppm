
export module cpu:ime;

namespace cpu
{
	export struct ime_state
	{
		bool enabled { false };
		bool requested { false };
		bool enabling { false };
	};
}
