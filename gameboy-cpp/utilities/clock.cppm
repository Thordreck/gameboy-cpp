
export module utilities:clock;
export import std;

namespace utils
{
    export template <std::regular_invocable Function>
    [[nodiscard]] auto measure_execution(Function&& f)
    {
        using clock = std::chrono::high_resolution_clock;
        const auto start = clock::now();

        f();

        const auto end = clock::now();
        return end - start;
    }

    export template <std::regular_invocable Function, typename Rep, typename Period>
    void execute_for_precise(Function&& f, const std::chrono::duration<Rep, Period>& duration)
    {
        using namespace std::chrono_literals;
        using clock = std::chrono::high_resolution_clock;

        const auto start = clock::now();
        const auto elapsed = measure_execution(std::forward<Function>(f));

        if (elapsed >= duration)
        {
            return;
        }

        const auto remaining_time = duration - elapsed;

        if (remaining_time > 1ms)
        {
            std::this_thread::sleep_for(remaining_time - 1ms);
        }

        const auto target_end_time = start + duration;
        while (clock::now() < target_end_time)
        {
            std::this_thread::yield();
        }
    }

    export template <std::regular_invocable Function, typename Rep, typename Period>
    void execute_for(Function&& f, const std::chrono::duration<Rep, Period>& duration)
    {
        const auto elapsed = measure_execution(std::forward<Function>(f));

        if (elapsed < duration)
        {
            const auto remaining_time = duration - elapsed;
            std::this_thread::sleep_for(remaining_time);
        }
    }

}