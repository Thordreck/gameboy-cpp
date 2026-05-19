
export module utilities:assert;
import std;

namespace utils
{
#ifndef NDEBUG
	export constexpr void assert(
		const bool condition,
		const char* message = nullptr,
		const std::source_location& loc = std::source_location::current())
	{
		if consteval
		{
			if (!condition)
			{
				throw std::runtime_error(message);
			}
		}
		else
		{
			if (!condition)
			{
				std::cerr << std::format("{}:{} Assertion failed. {}\n", loc.file_name(), loc.line(), message == nullptr ? "" : message);
				std::terminate();
			}
		}
	}
#else
	export constexpr void assert(const bool) {}

#endif

}
