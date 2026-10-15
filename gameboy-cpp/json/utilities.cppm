export module json:utilities;

import std;

import :common;
import :types;

namespace json
{
    export template<typename T, typename ...Args>
    [[nodiscard]] result<T> as(std::variant<Args...>&& variant)
    {
        return std::holds_alternative<T>(variant)
            ? result<T> { std::get<T>(variant) }
            : std::unexpected { std::format("Variant is not of type {}", typeid(T).name()) };
    }

    export template<typename T, typename ...Args>
    [[nodiscard]] result<T> as(const std::variant<Args...>& variant)
    {
        return std::holds_alternative<T>(variant)
            ? result<T> { std::get<T>(variant) }
            : std::unexpected { std::format("Variant is not of type {}", typeid(T).name()) };
    }

    export [[nodiscard]] std::optional<value> try_get(const object& jobject, const std::string_view key)
    {
        return jobject.has(key)
            ? std::make_optional(jobject.get(key))
            : std::nullopt;
    }

    export [[nodiscard]] result<value> get(const object& jobject, const std::string_view key)
    {
        return jobject.has(key)
            ? result<value>(jobject.get(key))
            : std::unexpected { std::format("Missing key {}", key) };
    }

}