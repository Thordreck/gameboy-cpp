
export module cpu:memory_bus;
import std;

namespace cpu
{
	export class memory_bus
	{
	public: 
		static constexpr std::size_t size = 0xFFFF;
		using data_t = std::span<std::uint8_t, size>;

	public:
		memory_bus(data_t view)
			: data { view }
		{}

		std::uint8_t operator [](const std::uint16_t index) const
		{
			return data[index];
		}

		std::uint8_t& operator [](const std::uint16_t index)
		{
			return data[index];
		}

	private:
		data_t data;
	};
}