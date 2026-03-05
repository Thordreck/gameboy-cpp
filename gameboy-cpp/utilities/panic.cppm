export module utilities:panic;

import std;

namespace utils
{
	// Taken from: https://buildingblock.ai/panic
	export template <class... Args>
	struct panic_format 
	{
		template <class T>
		consteval panic_format(const T& s, std::source_location loc = std::source_location::current()) noexcept
			: fmt{ s }
			, loc{ loc } 
		{}

		std::format_string<Args...> fmt;
		std::source_location loc;
	};

	export template <class... Args>
	[[noreturn]] void panic(panic_format<std::type_identity_t<Args>...> fmt,  Args &&...args) noexcept                         
	{                                                  
		auto msg = std::format(
			"{}:{} panic: {}\n", 
			fmt.loc.file_name(), 
			fmt.loc.line(),
			std::format(fmt.fmt, std::forward<Args>(args)...));

		std::cerr << msg;
		std::terminate();
	}
}
