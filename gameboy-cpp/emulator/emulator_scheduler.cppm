
export module emulator.scheduler;
import std;

namespace emulator
{
    export template<typename T>
    concept BatchSchedulable = requires(const T& const_instance, T& instance, const std::uint32_t ticks)
    {
        { const_instance.tick_batch() } -> std::convertible_to<std::uint32_t>;
        { instance.tick(ticks) } -> std::convertible_to<std::uint32_t>;
    };

    export template<BatchSchedulable... Components>
    class batch_scheduler
    {
    public:
        explicit batch_scheduler(Components&... components)
            : components(components...)
        {}

        void tick(const std::uint32_t ticks)
        {
            std::uint32_t remaining_ticks = ticks;

            while (remaining_ticks > 0)
            {
                const std::uint32_t batch = std::min(get_next_tick_batch(), remaining_ticks);
                std::apply([batch] (auto& c) { c.tick(batch); }, components);

                remaining_ticks -= batch;
            }
        }

    private:
        std::uint32_t get_next_tick_batch()
        {
            std::uint32_t min_ticks = std::numeric_limits<std::uint32_t>::max();

            std::apply(
                [&](auto&... c)
                {
                    ((min_ticks = std::min(min_ticks, c.tick_step())), ...);
                },
                components
            );

            return min_ticks;
        }

        std::tuple<Components&...> components;
    };

}