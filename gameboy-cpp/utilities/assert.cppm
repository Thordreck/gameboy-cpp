
export module utilities:assert;
export import std;

namespace utils
{
	constexpr bool debug =
#ifndef NDEBUG
		true;
#else
		false;
#endif

	export template<typename Callable>
	requires requires(Callable c) { { c() } -> std::convertible_to<bool>; }
	void assert(Callable&& condition, std::source_location loc = std::source_location::current())
	{
		if constexpr (debug)
		{
			if (!std::forward<decltype(condition)>(condition)())
			{
				auto msg = std::format("{}:{} Assertion failed\n", loc.file_name(), loc.line());
				std::cerr << msg;
				std::terminate();
			}
		}
	}
}
