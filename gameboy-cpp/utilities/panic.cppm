export module utilities:panic;

export import std;

namespace utils
{
	// Taken from: https://buildingblock.ai/panic
	export template <class... Args>
	struct panic_format 
	{
		template <class T>
		consteval panic_format(const T& s, const std::source_location loc = std::source_location::current()) noexcept
			: fmt{ s }
			, loc{ loc } 
		{}

		std::format_string<Args...> fmt;
		std::source_location loc;
	};

	export template <class... Args>
	[[noreturn]] void panic(panic_format<std::type_identity_t<Args>...> fmt,  Args &&...args) noexcept                         
	{                                                  
		const auto msg = std::format(
			"{}:{} panic: {}\n", 
			fmt.loc.file_name(), 
			fmt.loc.line(),
			std::format(fmt.fmt, std::forward<Args>(args)...));

		std::cerr << msg;
		std::terminate();
	}

	export template <typename T>
	T value_or_panic(std::expected<T, std::string> result, const std::source_location loc = std::source_location::current())
	{
		if (result.has_value())
		{
			return std::move(result).value();
		}

		const auto msg = std::format(
			"{}:{} panic: {}\n",
			loc.file_name(),
			loc.line(),
			result.error());

		std::cerr << msg;
		std::terminate();
	}

	export void panic_on_error(std::expected<void, std::string> result, const std::source_location loc = std::source_location::current())
	{
		if (!result.has_value())
		{
			const auto msg = std::format(
				"{}:{} panic: {}\n",
				loc.file_name(),
				loc.line(),
				result.error());

			std::cerr << msg;
			std::terminate();
		}
	}

}
