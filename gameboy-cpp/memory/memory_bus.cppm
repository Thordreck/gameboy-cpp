export module memory:bus;

export import std;
export import :common;
export import :map;

namespace memory
{
	export class memory_bus
	{
	public:
		memory_bus(memory_map_span_t map)
			: map{ map }
		{}

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

	export template<typename T>
	concept BusConnectable = requires(T instance, memory_bus & bus)
	{
		{ instance.connect(bus) } -> std::same_as<void>;
	};

	export template<BusConnectable... Connectables>
	void connect(memory_bus& bus, Connectables&... connectables)
	{
		(connectables.connect(bus), ...);
	}
}
