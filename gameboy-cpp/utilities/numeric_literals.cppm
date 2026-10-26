
export module utilities:numeric_literals;
import std;

namespace utils::numeric_literals
{
    // Adapted from https://medium.com/@nerudaj/tuesday-coding-tip-66-consteval-assertions-719098a77306
    inline void you_see_this_error_because_you_tried_to_initialize_an_integral_literal_with_a_value_lower_than_its_min() noexcept {}
    inline void you_see_this_error_because_you_tried_to_initialize_an_integral_literal_with_a_value_higher_than_its_max() noexcept {}

    consteval void consteval_assert(const bool condition, std::invocable auto&& error_function)
    {
        if (!condition)
        {
            error_function();
        }
    }

    template<typename To, typename From>
    requires std::is_integral_v<From> && std::is_integral_v<To>
    consteval To safe_integral_cast(const From value)
    {
        consteval_assert(
            value >= std::numeric_limits<To>::min(),
            you_see_this_error_because_you_tried_to_initialize_an_integral_literal_with_a_value_lower_than_its_min);

        consteval_assert(
            value <= std::numeric_limits<To>::max(),
            you_see_this_error_because_you_tried_to_initialize_an_integral_literal_with_a_value_higher_than_its_max);

        return static_cast<To>(value);
    }

    export consteval std::uint8_t operator""_u8(const unsigned long long int value)
    {
        return safe_integral_cast<std::uint8_t>(value);
    }

    export consteval std::uint16_t operator""_u16(const unsigned long long int value)
    {
        return safe_integral_cast<std::uint16_t>(value);
    }

}