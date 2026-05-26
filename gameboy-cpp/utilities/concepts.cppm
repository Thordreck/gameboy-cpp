
export module utilities:concepts;
import std;

namespace utils
{
    export template<typename T>
    concept decrementable = requires(T value) { --value; value--; };

    export template<typename T>
    concept addable = requires(T lhs, T rhs)
    {
        { lhs += rhs } -> std::same_as<T&>;
        { lhs + rhs } -> std::convertible_to<T>;
    };

    export template<typename T>
    concept substractable = requires(T lhs, T rhs)
    {
        { lhs -= rhs } -> std::same_as<T&>;
        { lhs - rhs } -> std::convertible_to<T>;
    };

    // Source - https://stackoverflow.com/a/61483494
    export template<class... Ts>
    concept all_same
        = sizeof...(Ts) < 2
        || std::conjunction_v<std::is_same<std::tuple_element_t<0, std::tuple<Ts...>>, Ts>...>;

}
