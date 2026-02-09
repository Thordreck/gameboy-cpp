
export module cpu:memory;
export import std;

namespace cpu
{
	export class memory
	{
	public:
		std::uint8_t operator [](const std::uint16_t index) const
		{
			return data[index];
		}

	private:
		std::array<std::uint8_t, 0xFFFF> data{};
	};
}
