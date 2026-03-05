export module memory:bus;

import :common;
import :map;

namespace memory
{
	export class memory_bus
	{
	public:
		memory_bus(memory_map_span_t map)
			: map{ map }
		{}

		memory_bus() = default;

		memory_data_t read(const memory_address_t address) const
		{
			const memory_map_page page = map[address >> 8];
			return page.read_func(page.context, address);
		}

		void write(const memory_address_t address, const memory_data_t value)
		{
			const memory_map_page page = map[address >> 8];
			page.write_func(page.context, address, value);
		}

	private:
		memory_map_span_t map;
	};
}
