
export module timer:tac;
import std;
import :common;

namespace timer
{
	export struct tac
	{
		bool enabled{ false };
		tac_clock clock{ tac_clock::_00 };
	};
}
