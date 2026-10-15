
module json:implementation;

import std;
import nlohmann.json;

import :types;
import :common;

namespace json
{
    struct internal
    {
        static [[nodiscard]] value to_json_value(const nlohmann::json& value);
        static [[nodiscard]] document to_json_document(const nlohmann::json& jdocument);

        static [[nodiscard]] std::string serialize(const object& jobject);
        static [[nodiscard]] std::string serialize(const array& jarray);

        static [[nodiscard]] nlohmann::json to_nlohmann_value(const array& value);
        static [[nodiscard]] nlohmann::json to_nlohmann_value(const object& value);
        static [[nodiscard]] nlohmann::json to_nlohmann_value(const value& value);
    };

    void to_json(nlohmann::json& j, const array& p) { j = internal::to_nlohmann_value(p); }
    void to_json(nlohmann::json& j, const object& p) { j = internal::to_nlohmann_value(p); }

    // array
    struct array::imp { nlohmann::json::array_t data; };

    array::array() : impl { std::make_unique<imp>() } {}
    array::~array() = default;

    array::array(const array& other) : impl { std::make_unique<imp>(other.impl->data) } {}
    array& array::operator=(const array& other)
    {
        impl->data = other.impl->data;
        return *this;
    }

    array::array(array&& other) noexcept : impl { std::exchange(other.impl, nullptr) } {}
    array& array::operator=(array&& other) noexcept
    {
        impl = std::exchange(other.impl, nullptr);
        return *this;
    }

    size_t array::size() const { return impl->data.size(); }
    value array::get(const size_t idx) const { return internal::to_json_value(impl->data[idx]); }
    void array::set(size_t idx, const value& value) { impl->data[idx] = internal::to_nlohmann_value(value); }
    void array::push_back(const value& value) { impl->data.push_back(internal::to_nlohmann_value(value)); }

    // object
    struct object::imp { nlohmann::json::object_t data; };

    object::object() : impl { std::make_unique<imp>() } {}
    object::~object() = default;

    object::object(const object& other) : impl { std::make_unique<imp>(other.impl->data) } {}
    object& object::operator=(const object& other)
    {
        impl->data = other.impl->data;
        return *this;
    }

    object::object(object&& other) noexcept : impl { std::exchange(other.impl, nullptr) } {}
    object& object::operator=(object&& other) noexcept
    {
        impl = std::exchange(other.impl, nullptr);
        return *this;
    }

    size_t object::size() const { return impl->data.size(); }
    value object::get(const std::string_view key) const { return internal::to_json_value(impl->data[std::string(key)]); }
    void object::set(const std::string_view key, const value& value) { impl->data[std::string(key)] = internal::to_nlohmann_value(value); }

    // Serialization
    [[nodiscard]] result<document> deserialize(const std::span<const std::uint8_t> data)
    {
        try
        {
            const auto deserialized = nlohmann::json::parse(data);
            return internal::to_json_document(deserialized);
        }
        catch (const nlohmann::json::parse_error& ex)
        {
            return std::unexpected { ex.what() };
        }
    }

    [[nodiscard]] result<std::vector<std::uint8_t>> serialize(const document& doc)
    {
        try
        {
            const std::string str_serialization = std::visit([] (auto&& jvalue) { return internal::serialize(jvalue); }, doc);
            return std::vector<std::uint8_t> { str_serialization.begin(), str_serialization.end() };
        }
        catch (nlohmann::json::exception& ex)
        {
            return std::unexpected { ex.what() };
        }
    }

    // Helpers
    [[nodiscard]] value internal::to_json_value(const nlohmann::json& value)
    {
        using enum nlohmann::json::value_t;

        switch (value.type())
        {
        case null:
            return json::null {};
        case boolean:
            return value.get<bool>();
        case number_integer:
        case number_unsigned:
        case number_float:
            return value.get<double>();
        case string:
            return value.get<std::string>();
        case array:
            {
                json::array jarray {};
                jarray.impl->data = value.get<nlohmann::json::array_t>();
                return jarray;
            }
        case object:
            {
                json::object jobject {};
                jobject.impl->data = value.get<nlohmann::json::object_t>();
                return jobject;
            }
        default:
            std::unreachable();
        }
    }

    [[nodiscard]] document internal::to_json_document(const nlohmann::json& jdocument)
    {
        if (jdocument.is_object())
        {
            object jobject {};
            jobject.impl->data = jdocument.get<nlohmann::json::object_t>();

            return jobject;
        }

        array jarray {};
        jarray.impl->data = jdocument.get<nlohmann::json::array_t>();

        return jarray;
    }

    [[nodiscard]] std::string internal::serialize(const object& jobject) { return nlohmann::json(jobject.impl->data).dump(); }
    [[nodiscard]] std::string internal::serialize(const array& jarray) { return nlohmann::json(jarray.impl->data).dump(); }

    [[nodiscard]] nlohmann::json internal::to_nlohmann_value(const array& value) { return value.impl->data; }
    [[nodiscard]] nlohmann::json internal::to_nlohmann_value(const object& value) { return value.impl->data; }
    [[nodiscard]] nlohmann::json internal::to_nlohmann_value(const value& value)
    {
        return std::visit([] (auto&& input) { return nlohmann::json(input); }, value);
    }

}
