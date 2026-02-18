
export module cpu:memory_bus;
import std;

namespace cpu
{
	export class memory_bus
	{
	public: 
		static constexpr std::size_t size = 0x10000;

		using type_t = std::uint8_t;
		using index_t = std::uint16_t;
		using data_t = std::span<type_t, size>;

	public:
		memory_bus(data_t view)
			: data { view }
		{}

		type_t operator [](const index_t index) const
		{
			return data[index];
		}

		type_t& operator [](const index_t index)
		{
			return data[index];
		}

	private:
		data_t data;
	};
}