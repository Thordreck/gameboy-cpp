
export module utilities:memory;
import std;

namespace utils
{
	export template<typename T>
	concept ByteSized = sizeof(T) == 1;

	export template<typename Input>
	requires ByteSized<Input> && std::unsigned_integral<Input>
	std::uint16_t constexpr encode_little_endian(const Input least_significant, const Input most_significant)
	{
		return (static_cast<std::uint16_t>(most_significant) << 8) | static_cast<std::uint16_t>(least_significant);
	}
}