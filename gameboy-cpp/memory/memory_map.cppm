
export module memory:map;
export import std;
export import :common;

namespace memory
{
	export using memory_region_read_fn_t = memory_data_t(*)(const void*, const memory_address_t);
	export using memory_region_write_fn_t = void(*)(void*, const memory_address_t, const memory_data_t);

	export constexpr std::uint16_t memory_map_page_size = 256;
	export constexpr std::uint16_t memory_map_num_pages = memory_size / memory_map_page_size;

	export struct memory_map_page
	{
		memory_region_read_fn_t read_func;
		memory_region_write_fn_t write_func;
		void* context;
	};

	export using memory_map_span_t = std::span<memory_map_page, memory_map_num_pages>;
	export using memory_map_array_t = std::array<memory_map_page, memory_map_num_pages>;

	export template<typename T>
	concept MappedRegion =
		std::convertible_to<decltype(T::start), memory_address_t>
		&& std::convertible_to<decltype(T::end), memory_address_t>
		&& (T::start <= T::end)
		&& ((T::end - T::start + 1) % memory_map_page_size == 0);

	export template<typename T>
	concept MappedMemoryRegion = Memory<T> && MappedRegion<T>;

	template<ReadOnlyMemory T>
	static memory_data_t read_mapped_region(const void* context, const memory_address_t address)
	{
		const T* self = static_cast<const T*>(context);
		return self->read(address);
	}

	template<WriteOnlyMemory T>
	static void write_mapped_region(void* context, const memory_address_t address, const memory_data_t value)
	{
		T* self = static_cast<T*>(context);
		self->write(address, value);
	}

	template<MappedMemoryRegion... Regions>
	static consteval bool regions_cover_all_memory_addresses_with_no_overlap()
	{
		std::array regions{ std::pair { Regions::start, Regions::end }... };
		std::ranges::sort(regions, [](auto lhs, auto rhs) { return lhs.first < rhs.first; });

		std::size_t current = 0;

		for (auto [start, end] : regions)
		{
			if (start != current)
			{
				return false;
			}

			current = end + 1;
		}

		return current == memory_size;
	}

	export template<MappedMemoryRegion... Regions>
	auto build_memory_map(Regions&... regions)
	{
		static_assert(
			regions_cover_all_memory_addresses_with_no_overlap<Regions...>(), 
			"Mapped memory regions must cover all memory addresses with no overlap");

		memory_map_array_t mapping{};

		auto fill_region = [&]<typename T>(T& region)
		{
			using region_t = std::remove_reference_t<T>;

			const memory_map_page page
			{
				read_mapped_region<region_t>,
				write_mapped_region<region_t>,
				&region,
			};

			constexpr size_t initial_page_pos = region_t::start >> 8;
			const auto initial_fill_pos = std::next(mapping.begin(), initial_page_pos);

			std::ranges::fill_n(initial_fill_pos, num_pages, page);
		};

		(fill_region(regions), ...); 
		return mapping;
	}

	export template<size_t size, memory_address_t start_address = 0, memory_address_t end_address = size - 1> 
	requires (size == (end_address - start_address + 1))
	class span_map
	{
	public:
		static constexpr auto start = start_address;
		static constexpr auto end = end_address;

		explicit span_map(std::span<memory_data_t, size> data)
			: data{ data }
		{}

		[[nodiscard]] memory_data_t read(const memory_address_t address) const
		{
			return data[address - start];
		}

		void write(const memory_address_t address, const memory_data_t value) 
		{
			data[address - start] = value;
		}

	private:
		std::span<memory_data_t, size> data;
	};

	export template<size_t size, memory_address_t start_address = 0, memory_address_t end_address = size - 1> 
	requires (size == (end_address - start_address + 1))
	constexpr auto map(std::span<memory_data_t, size> data)
	{
		return span_map<size, start_address, end_address>{ data };
	}

	export template<size_t size, memory_address_t start_address = 0, memory_address_t end_address = size - 1> 
	requires (size == (end_address - start_address + 1))
	constexpr auto map(std::array<memory_data_t, size>& data)
	{
		return map<size, start_address, end_address>(std::span(data));
	}
}
