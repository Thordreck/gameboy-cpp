
export module emulator.engine:scheduler;
import std;

namespace emulator
{
    export template<typename T>
    concept BatchSchedulable = requires(const T& const_instance, T& instance, const std::uint32_t ticks)
    {
        { const_instance.active() } -> std::convertible_to<bool>;
        { const_instance.tick_batch() } -> std::convertible_to<std::uint32_t>;
        { instance.tick(ticks) } -> std::same_as<void>;
    };

    template<BatchSchedulable... Components>
    static std::uint32_t get_next_tick_batch(const Components&... components)
    {
        std::uint32_t min_ticks = std::numeric_limits<std::uint32_t>::max();
        ((min_ticks = std::min(min_ticks, components.tick_batch())), ...);

        return min_ticks;
    }

    export template<typename Component, typename... Args>
    struct scheduler_adapter
    {
        Component& component;
        std::tuple<Args&...> args;

        [[nodiscard]] bool active() const { return component.active(); }
        [[nodiscard]] std::uint32_t tick_batch() const { return component.tick_batch(); }

        void tick(const std::uint32_t ticks)
        {
            std::apply([this, ticks] (auto&... args) { component.tick(ticks, args...); }, args);
        }
    };

    template<typename Component, typename... Args>
    auto adapt_for_scheduler(Component& c, Args&... args)
    {
        return scheduler_adapter<Component, Args...> { c, std::tie(args...) };
    }

    export template<BatchSchedulable... Components>
    void batch_schedule(const std::uint32_t ticks, Components&&... components)
    {
        std::uint32_t remaining_ticks = ticks;

        while (remaining_ticks > 0)
        {
            const std::uint32_t batch = std::min(get_next_tick_batch(components...), remaining_ticks);
            ((components.active() && (components.tick(batch), true)), ...);
            remaining_ticks -= batch;
        }
    }

}