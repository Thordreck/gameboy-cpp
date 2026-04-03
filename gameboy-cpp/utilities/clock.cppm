
export module utilities:clock;
export import std;

namespace utils
{
    export template <std::regular_invocable Function>
    [[nodiscard]] auto execution_time(Function&& f)
    {
        using clock = std::chrono::high_resolution_clock;
        const auto start = clock::now();

        f();

        const auto end = clock::now();
        return end - start;
    }

}