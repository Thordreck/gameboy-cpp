
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
				const char* file_name = loc.file_name();
				const auto line = loc.line();
				const char* effective_message = message == nullptr ? "" : message;

				std::cerr << std::vformat(
					"{}:{} Assertion failed. {}\n",
					std::make_format_args(file_name, line, effective_message));

				std::terminate();
			}
		}
	}
#else
	export constexpr void assert(const bool, const char* message = nullptr) {}

#endif

}
