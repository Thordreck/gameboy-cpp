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

    using access_policy_can_read_fn_t = bool(*)(const void*, memory_address_t);
    using access_policy_can_write_fn_t = bool(*)(const void*, memory_address_t);
    using access_destroy_fn_t = void(*)(void*);

    template <AccessPolicy T>
    [[nodiscard]] static bool invoke_can_read(const void* context, const memory_address_t address)
    {
        const T* policy = static_cast<const T*>(context);
        return policy->can_read(address);
    }

    template <AccessPolicy T>
    [[nodiscard]] static bool invoke_can_write(const void* context, const memory_address_t address)
    {
        const T* policy = static_cast<const T*>(context);
        return policy->can_write(address);
    }

    template <AccessPolicy T>
    static void invoke_destroy(void* context)
    {
        T* policy = static_cast<T*>(context);
        std::destroy_at(policy);
    }

    class access_policy
    {
    public:
        template <AccessPolicy Policy>
        requires (sizeof(std::decay_t<Policy>) <= 64)
            && (alignof(std::decay_t<Policy>) <= alignof(std::max_align_t))
            && std::destructible<std::decay_t<Policy>>
        explicit access_policy(Policy&& policy)
            : can_read_func{invoke_can_read<std::decay_t<Policy>>}
            , can_write_func{invoke_can_write<std::decay_t<Policy>>}
            , destroy_func{invoke_destroy<std::decay_t<Policy>>}
            , context {}
        {
            using U = std::decay_t<Policy>;
            new(context.data()) U(std::forward<Policy>(policy));
        }

        ~access_policy()
        {
            destroy_func(context.data());
        }

        access_policy(const access_policy&) = delete;
        access_policy& operator=(const access_policy&) = delete;

        [[nodiscard]] bool can_read(const memory_address_t address) const
        {
            return can_read_func(context.data(), address);
        }

        [[nodiscard]] bool can_write(const memory_address_t address) const
        {
            return can_write_func(context.data(), address);
        }

    private:
        access_policy_can_read_fn_t can_read_func;
        access_policy_can_write_fn_t can_write_func;
        access_destroy_fn_t destroy_func;

        alignas(std::max_align_t) std::array<std::byte, 64> context;
    };

    template<AccessPolicy... Policies>
    class access_policy_chain
    {
    public:
        explicit access_policy_chain(Policies&&... policies)
            : policies { std::forward<Policies>(policies)... }
        {}

        [[nodiscard]] bool can_read(const memory_address_t address) const
        {
            return can_read_impl(address, std::index_sequence_for<Policies...>{});
        }

        [[nodiscard]] bool can_write(const memory_address_t address) const
        {
            return can_write_impl(address, std::index_sequence_for<Policies...>{});
        }

    private:
        template<std::size_t... I>
        [[nodiscard]] bool can_read_impl(const memory_address_t address, std::index_sequence<I...>) const
        {
            return (... && std::get<I>(policies).can_read(address));
        }

        template<std::size_t... I>
        [[nodiscard]] bool can_write_impl(const memory_address_t address, std::index_sequence<I...>) const
        {
            return (... && std::get<I>(policies).can_write(address));
        }

        using tuple_t = std::tuple<std::decay_t<Policies>...>;
        tuple_t policies;
    };


    struct free_access_policy
    {
        [[nodiscard]] bool can_read(const memory_address_t) const { return true; }
        [[nodiscard]] bool can_write(const memory_address_t) const { return true; }
    };

    export class memory_bus
    {
    public:
        template <AccessPolicy Policy>
        explicit memory_bus(const memory_map_span_t map, Policy&& policy)
            : map{map}
            , access(std::forward<Policy>(policy))
        {}

        template <AccessPolicy... Policies>
        requires (sizeof...(Policies) > 1)
        explicit memory_bus(const memory_map_span_t map, Policies&&... policies)
            : map{map}
            , access(access_policy_chain<Policies...>(std::forward<Policies>(policies)...))
        {}

        explicit memory_bus(const memory_map_span_t map)
            : map{map}
            , access(free_access_policy {})
        {}

        [[nodiscard]] memory_data_t read(const memory_address_t address) const
        {
            {
                PROFILER_SCOPE("Memory bus policy");
                if (!access.can_read(address))
                {
                    return 0xFF;
                }
            }

            PROFILER_SCOPE("Memory bus read");
            const memory_map_page& page = map[address >> 8];
            return page.read_func(page.context, address);
        }

        void write(const memory_address_t address, const memory_data_t value)
        {
            if (access.can_write(address))
            {
                const memory_map_page& page = map[address >> 8];
                page.write_func(page.context, address, value);
            }
        }

    private:
        memory_map_span_t map;
        access_policy access;
    };

    export template <typename T>
    concept BusConnectable = requires(T instance, memory_bus& bus)
    {
        { instance.connect(bus) } -> std::same_as<void>;
    };

    export template <BusConnectable... Connectables>
    void connect(memory_bus& bus, Connectables&... connectables)
    {
        (connectables.connect(bus), ...);
    }

    export template <memory_address_t start, memory_address_t end>
    requires (start <= end)
    class read_only_memory_policy
    {
    public:
        [[nodiscard]] bool can_read(const memory_address_t _) const { return true; }
        [[nodiscard]] bool can_write(const memory_address_t address) const { return !is_in_region<start, end>(address); }
    };

}
