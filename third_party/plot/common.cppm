export module plot:common;
import std;

namespace plot
{
    export template <typename T>
    using result_t = std::expected<T, std::string>;

    export using outcome_t = result_t<void>;

    export template<typename Container, typename Value>
    concept Indexable = requires(Container container, size_t index)
    {
        { container[index] } -> std::convertible_to<Value>;
    };

    export template<typename Container>
    concept WithSize = requires(Container container)
    {
        { container.size() } -> std::convertible_to<size_t>;
    };

    export template<typename Container, typename Value>
    concept IndexableWithSize = Indexable<Container, Value> && WithSize<Container>;

    export template<typename Fn, typename R, typename... Args>
    concept IsInvocableR = std::is_invocable_r_v<R, Fn, Args...>;

}