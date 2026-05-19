
export module utilities:constrained;

import std;
import :assert;
import :concepts;

namespace utils
{
    export template<typename T, T lower_bound, T upper_bound>
    requires std::three_way_comparable<T> && (lower_bound <= upper_bound)
    class constrained
    {
    public:
        static constexpr auto min = lower_bound;
        static constexpr auto max = upper_bound;

        constexpr constrained() requires std::is_default_constructible_v<T>
            : value {}
        {
            check_boundaries();
        }

        explicit constexpr constrained(const T& value) requires std::is_copy_constructible_v<T>
            : value { value }
        {
            check_boundaries();
        }

        explicit constexpr constrained(T&& value) requires std::is_move_constructible_v<T>
            : value { std::move(value) }
        {
            check_boundaries();
        }

        [[nodiscard]] constexpr T raw() const { return value; }
        [[nodiscard]] constexpr explicit operator T() const { return raw(); }

        template<typename Other>
        requires std::three_way_comparable_with<T, Other>
        [[nodiscard]] constexpr auto operator<=>(const Other& other) const { return value <=> other; }

        template<typename Other>
        requires std::equality_comparable_with<T, Other>
        [[nodiscard]] constexpr bool operator==(const Other& other) const
        {
            return value == other;
        }

        constexpr auto& operator++() requires std::incrementable<T>
        {
            ++value;
            check_boundaries();

            return *this;
        }

        constexpr auto operator++(int) requires std::incrementable<T>
        {
            const auto old = *this;
            operator++();
            check_boundaries();

            return old;
        }

        constexpr auto& operator--() requires decrementable<T>
        {
            --value;
            check_boundaries();

            return *this;
        }

        constexpr auto operator--(int) requires decrementable<T>
        {
            const auto old = *this;
            operator--();
            check_boundaries();

            return old;
        }

        template<typename Other>
        requires addable<T> && std::convertible_to<Other, T>
        constexpr auto& operator+=(const Other& other)
        {
            value = value + other;
            check_boundaries();

            return *this;
        }

        template<typename Other>
        requires addable<T> && std::convertible_to<Other, T>
        constexpr auto operator+(const Other& other) const
        {
            return constrained { value + other };
        }

        template<typename Other>
        requires substractable<T> && std::convertible_to<Other, T>
        constexpr auto& operator-=(const Other& other)
        {
            value = value - other;
            check_boundaries();

            return *this;
        }

        template<typename Other>
        requires substractable<T> && std::convertible_to<Other, T>
        constexpr auto operator-(const Other& other) const
        {
            return constrained { value - other };
        }

        template<typename Other>
        requires std::is_copy_assignable_v<T> && std::convertible_to<Other, T>
        constexpr auto& operator=(const Other& other)
        {
            value = other;
            check_boundaries();

            return *this;
        }

    private:
        constexpr void check_boundaries() const { assert(value >= min && value <= max); }

        T value;
    };

}