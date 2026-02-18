
export module memory;
import std;

namespace cpu
{
	export class memory
	{
	public:
		std::uint8_t operator [](const std::uint16_t index) const
		{
			return data[index];
		}

		std::uint8_t& operator [](const std::uint16_t index)
		{
			return data[index];
		}

	private:
		std::array<std::uint8_t, 0x10000> data{};
	};
}
