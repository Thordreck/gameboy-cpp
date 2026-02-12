
export module utilities:memory;
export import std;

namespace utils
{
	export template<typename T>
	concept ByteSized = sizeof(T) == 1;

	export template<typename Collection, typename Index>
	concept ByteIndexable = requires(const Collection& collection, const Index index)
	{
		{ collection[index] } -> std::convertible_to<std::uint8_t>;
	};

	export template<typename Index>
	concept Incrementable = requires(const Index index)
	{
		{ index + 1 } -> std::convertible_to<Index>;
	};

	export template<typename Input>
	requires ByteSized<Input> && std::unsigned_integral<Input>
	std::uint16_t constexpr encode_little_endian(const Input least_significant, const Input most_significant)
	{
		return (static_cast<std::uint16_t>(most_significant) << 8) | static_cast<std::uint16_t>(least_significant);
	}

	export template<typename Collection, typename Index>
	requires ByteIndexable<Collection, Index> && Incrementable<Index>
	std::uint16_t constexpr read_two_byte_little_endian(const Collection& collection, const Index index)
	{
		return encode_little_endian(collection[index], collection[index + 1]);
	}
}