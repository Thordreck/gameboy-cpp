module;
#include "profiling.hpp"

export module memory:bus;

export import std;
export import :common;
export import :map;

namespace memory
{
    export template <typename T>
    concept AccessPolicy = requires(const T& instance, const memory_address_t address)
    {
        { instance.can_read(address) } -> std::convertible_to<bool>;
        { instance.can_write(address) } -> std::convertible_to<bool>;
    };

    export template <Memory Map, AccessPolicy... Policies>
    class memory_bus
    {
    public:
        explicit memory_bus(Map& map, Policies&... policies)
            : map { map }
            , policies { policies... }
        {}

        [[nodiscard]] memory_data_t read(const memory_address_t address) const
        {
            PROFILER_SCOPE("Memory bus read");
            return can_read(address) ? map.read(address) : 0xFF;
        }

        void write(const memory_address_t address, const memory_data_t value)
        {
            PROFILER_SCOPE("Memory bus write");

            if (can_write(address))
            {
                map.write(address, value);
            }
        }

    private:
        [[nodiscard]] bool can_read(const memory_address_t address) const
        {
            PROFILER_SCOPE("Memory bus read policy");
            return std::apply([address](const auto&... policy) { return (policy.can_read(address) && ...); }, policies);
        }

        [[nodiscard]] bool can_write(const memory_address_t address) const
        {
            return std::apply([address](const auto&... policy) { return (policy.can_write(address) && ...); }, policies);
        }

        Map& map;
        std::tuple<Policies&...> policies;
    };

    export template <memory_address_t start, memory_address_t end>
    requires (start <= end)
    class read_only_memory_policy
    {
    public:
        [[nodiscard]] static bool can_read(const memory_address_t _) { return true; }
        [[nodiscard]] static bool can_write(const memory_address_t address) { return !is_in_region<start, end>(address); }
    };

    export struct free_access_policy
    {
        [[nodiscard]] static bool can_read(const memory_address_t) { return true; }
        [[nodiscard]] static bool can_write(const memory_address_t) { return true; }
    };

}
