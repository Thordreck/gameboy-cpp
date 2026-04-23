export module memory:map;
export import :common;
export import std;

namespace memory
{
    export constexpr std::uint16_t memory_map_page_size = 256;
    export constexpr std::uint16_t memory_map_num_pages = memory_size / memory_map_page_size;

    export template <typename T>
    concept MappedRegion =
        std::convertible_to<decltype(T::start), memory_address_t>
        && std::convertible_to<decltype(T::end), memory_address_t>
        && (T::start <= T::end)
        && ((T::end - T::start + 1) % memory_map_page_size == 0);

    export template <typename T>
    concept MappedMemoryRegion = Memory<T> && MappedRegion<T>;

    template <MappedRegion... Regions>
    static consteval bool regions_cover_all_memory_addresses_with_no_overlap()
    {
        std::array regions{std::pair{Regions::start, Regions::end}...};
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

    template <MappedRegion... Regions>
    static auto build_regions_index_mapping(const std::tuple<Regions&...>& regions)
    {
        std::array<std::uint8_t, memory_map_num_pages> mapping {};

        const auto fill_mappings = [&]<std::size_t ...Is>(std::index_sequence<Is...>)
        {
            ([&]
            {
                using region_t = std::decay_t<decltype(std::get<Is>(regions))>;
                constexpr std::size_t num_pages = (region_t::end - region_t::start + 1) / memory_map_page_size;

                constexpr std::uint8_t initial_page_pos = region_t::start >> 8;
                const auto initial_fill_pos = std::next(mapping.begin(), initial_page_pos);

                std::ranges::fill_n(initial_fill_pos, num_pages, Is);
            }(), ...);
        };

        fill_mappings(std::make_index_sequence<sizeof...(Regions)>{});
        return mapping;
    }

    // TODO: this only work for the current memory map compilation
#define REPEAT_256(X) \
X(0)   X(1)   X(2)   X(3)   X(4)   X(5)   X(6)   X(7)   X(8)

#define GEN_READ_CASE(i) \
case i: return std::get<i>(regions).read(address);

#define GEN_WRITE_CASE(i) \
case i: std::get<i>(regions).write(address, value); break;

    export template <MappedMemoryRegion... Regions>
    class memory_map
    {
        static_assert(
            regions_cover_all_memory_addresses_with_no_overlap<Regions...>(),
            "Mapped memory regions must cover all memory addresses with no overlap");

    public:
        explicit memory_map(Regions&... memory_regions)
            : regions{memory_regions...}
            , mapping{build_regions_index_mapping(regions)}
        {}

        memory_map(const memory_map&) = delete;
        memory_map& operator=(const memory_map&) = delete;

        [[nodiscard]] memory_data_t read(const memory_address_t address) const
        {
            const std::uint8_t region_index = mapping[address >> 8];

            switch (region_index)
            {
                REPEAT_256(GEN_READ_CASE)
            default:
                std::unreachable();
            }
        }

        void write(const memory_address_t address, const memory_data_t value)
        {
            const std::uint8_t region_index = mapping[address >> 8];

            switch (region_index)
            {
                REPEAT_256(GEN_WRITE_CASE)
            default:
                std::unreachable();
            }
        }

    private:
        std::tuple<Regions&...> regions;
        std::array<std::uint8_t, memory_map_num_pages> mapping;
    };

    export template <size_t size, memory_address_t start_address = 0, memory_address_t end_address = size - 1>
        requires (size == (end_address - start_address + 1))
    class span_map
    {
    public:
        static constexpr auto start = start_address;
        static constexpr auto end = end_address;

        explicit span_map(std::span<memory_data_t, size> data)
            : data{data}
        {
        }

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

    export template <size_t size, memory_address_t start_address = 0, memory_address_t end_address = size - 1>
        requires (size == (end_address - start_address + 1))
    constexpr auto map(std::span<memory_data_t, size> data)
    {
        return span_map<size, start_address, end_address>{data};
    }

    export template <size_t size, memory_address_t start_address = 0, memory_address_t end_address = size - 1>
        requires (size == (end_address - start_address + 1))
    constexpr auto map(std::array<memory_data_t, size>& data)
    {
        return map<size, start_address, end_address>(std::span(data));
    }
}
