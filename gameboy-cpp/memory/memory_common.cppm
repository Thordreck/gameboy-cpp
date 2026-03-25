
export module memory:common;
export import std;

namespace memory
{
	export constexpr size_t memory_size = 0x10000;

	export using memory_data_t = std::uint8_t;
	export using memory_address_t = std::uint16_t;

	export template<typename T>
	concept ReadOnlyMemory = requires(const T& memory, const memory_address_t address)
	{
		{ memory.read(address) } -> std::convertible_to<memory_data_t>;
	};

	export template<typename T>
	concept WriteOnlyMemory = requires(T& memory, const memory_address_t address, const memory_data_t value)
	{
		{ memory.write(address, value) } -> std::same_as<void>;
	};

	export template<typename T>
	concept Memory = WriteOnlyMemory<T> && ReadOnlyMemory<T>;

	export template<memory_address_t start, memory_address_t end>
	requires (start <= end)
	[[nodiscard]] bool is_in_region(const memory_address_t address)
	{
		return address >= start && address <= end;
	}

}
