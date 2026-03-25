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

    using access_policy_can_read_fn_t = bool(*)(const void*, const memory_address_t);
    using access_policy_can_write_fn_t = bool(*)(const void*, const memory_address_t);
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
        requires (sizeof(std::decay_t<Policy>) <= 32)
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

        alignas(std::max_align_t) std::array<std::byte, 32> context;
    };

    template<AccessPolicy... Policies>
    class access_policy_chain
    {
    public:
        access_policy_chain(Policies&&... policies)
            : policies { std::forward<Policies>(policies)... }
        {}

        [[nodiscard]] bool can_read(const memory_address_t address) const
        {
            return std::apply([address] (const auto&... policy) { return (... && policy.can_read(address)); }, policies);
        }

        [[nodiscard]] bool can_write(const memory_address_t address) const
        {
            return std::apply([address] (const auto&... policy) { return (... && policy.can_write(address)); }, policies);
        }

    private:
        using tuple_t = std::tuple<std::decay_t<Policies>...>;
        tuple_t policies;
    };

    export class memory_bus
    {
    public:
        template <AccessPolicy Policy>
        memory_bus(const memory_map_span_t map, Policy&& policy)
            : map{map}
            , access(std::in_place, std::forward<Policy>(policy))
        {}

        template <AccessPolicy... Policies>
        requires (sizeof...(Policies) > 1)
        memory_bus(const memory_map_span_t map, Policies&&... policies)
            : memory_bus(map, access_policy_chain<Policies...>(std::forward<Policies>(policies)...))
        {}

        memory_bus(const memory_map_span_t map)
            : map{map}
            , access(std::nullopt)
        {}

        [[nodiscard]] memory_data_t read(const memory_address_t address) const
        {
            const bool has_access = access
                .transform([address] (const auto& policy){ return policy.can_read(address); })
                .value_or(true);

            if (!has_access)
            {
                return 0xFF;
            }

            const memory_map_page page = map[address >> 8];
            return page.read_func(page.context, address);
        }

        void write(const memory_address_t address, const memory_data_t value)
        {
            const bool has_access = access
                .transform([address] (const auto& policy){ return policy.can_write(address); })
                .value_or(true);

            if (has_access)
            {
                const memory_map_page page = map[address >> 8];
                page.write_func(page.context, address, value);
            }
        }

    private:
        memory_map_span_t map;
        std::optional<access_policy> access;
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
}
