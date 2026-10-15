#include "doctest.h"

import json;
import std;

template <typename TExpected, typename TError>
[[nodiscard]] static TExpected required(std::expected<TExpected, TError>&& result)
{
    REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
    return std::forward<TExpected>(result.value());
}

template <typename TError>
static void required(std::expected<void, TError>&& result)
{
    REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
}

template <typename TExpected, typename ...TArgs>
[[nodiscard]] static const TExpected& required(const std::variant<TArgs...>& variant)
{
    REQUIRE_MESSAGE(std::holds_alternative<TExpected>(variant), std::format("Variant is missing type {}", typeid(TExpected).name()));
    return std::get<TExpected>(variant);
}

TEST_CASE("json.Empty json can be parsed properly")
{
    using namespace json;

    const auto jdoc = required(deserialize("{}"));
    const auto jobject = required<object>(jdoc);

    REQUIRE_EQ(jobject.size(), 0);
}

TEST_CASE("json.Json with simple value types can be parsed properly")
{
    using namespace json;

    const auto jdoc = required(deserialize(R"(
    {
        "number_value": 4.19,
        "bool_value": true,
        "string_value": "Hello, world"
    })"));

    const auto jobject = required<object>(jdoc);

    REQUIRE_EQ(jobject.size(), 3);
    REQUIRE_EQ(required<double>(jobject.get("number_value")), 4.19);
    REQUIRE_EQ(required<bool>(jobject.get("bool_value")), true);
    REQUIRE_EQ(required<string>(jobject.get("string_value")), "Hello, world");
}

TEST_CASE("json.Json with array with heterogenous value types can be parsed properly")
{
    using namespace json;

    const auto jdoc = required(deserialize(R"(
    {
        "array_value": [ 4.19, true, "Hello, world" ]
    })"));

    const auto jobject = required<object>(jdoc);
    REQUIRE_EQ(jobject.size(), 1);

    const auto jarray = required<array>(jobject.get("array_value"));
    REQUIRE_EQ(jarray.size(), 3);

    REQUIRE_EQ(required<double>(jarray.get(0)), 4.19);
    REQUIRE_EQ(required<bool>(jarray.get(1)), true);
    REQUIRE_EQ(required<string>(jarray.get(2)), "Hello, world");
}

TEST_CASE("json.Json with object with heterogenous value types can be parsed properly")
{
    using namespace json;

    const auto jdoc = required(deserialize(R"(
    {
        "object_value":
        {
            "number_value": 4.19,
            "bool_value": true,
            "string_value": "Hello, world"
        }
    })"));

    const auto jobject = required<object>(jdoc);
    REQUIRE_EQ(jobject.size(), 1);

    const auto inner_jobject = required<object>(jobject.get("object_value"));
    REQUIRE_EQ(inner_jobject.size(), 3);

    REQUIRE_EQ(inner_jobject.size(), 3);
    REQUIRE_EQ(required<double>(inner_jobject.get("number_value")), 4.19);
    REQUIRE_EQ(required<bool>(inner_jobject.get("bool_value")), true);
    REQUIRE_EQ(required<string>(inner_jobject.get("string_value")), "Hello, world");
}

TEST_CASE("json.Empty json can be serialized properly")
{
    using namespace json;

    object jobject {};
    const auto serialized = required(serialize(jobject));
    const auto deserialized = required<object>(required(deserialize(serialized)));

    REQUIRE_EQ(deserialized.size(), 0);
}

TEST_CASE("json.Json with simple value types can be serialized properly")
{
    using namespace json;

    object jobject {};
    jobject.set("number_value", 4.19);
    jobject.set("bool_value", true);
    jobject.set("string_value", "Hello, world");

    const auto serialized = required(serialize(jobject));
    const auto deserialized = required<object>(required(deserialize(serialized)));

    REQUIRE_EQ(deserialized.size(), 3);
    REQUIRE_EQ(required<double>(deserialized.get("number_value")), 4.19);
    REQUIRE_EQ(required<bool>(deserialized.get("bool_value")), true);
    REQUIRE_EQ(required<string>(deserialized.get("string_value")), "Hello, world");
}

TEST_CASE("json.Json with array with heterogenous value types can be serailized properly")
{
    using namespace json;

    object jobject {};
    array jarray {};
    jarray.push_back(4.19);
    jarray.push_back(true);
    jarray.push_back("Hello, world");
    jobject.set("array_value", jarray);

    const auto serialized = required(serialize(jobject));
    const auto deserialized = required<object>(required(deserialize(serialized)));
    REQUIRE_EQ(deserialized.size(), 1);

    const auto deserialized_jarray = required<array>(deserialized.get("array_value"));
    REQUIRE_EQ(deserialized_jarray.size(), 3);

    REQUIRE_EQ(required<double>(deserialized_jarray.get(0)), 4.19);
    REQUIRE_EQ(required<bool>(deserialized_jarray.get(1)), true);
    REQUIRE_EQ(required<string>(deserialized_jarray.get(2)), "Hello, world");
}

TEST_CASE("json.Json with object with heterogenous value types can be serialized properly")
{
    using namespace json;

    object jobject {};
    object inner_jobject {};

    inner_jobject.set("number_value", 4.19);
    inner_jobject.set("bool_value", true);
    inner_jobject.set("string_value", "Hello, world");

    jobject.set("object_value", inner_jobject);

    const auto serialized = required(serialize(jobject));
    const auto deserialized = required<object>(required(deserialize(serialized)));
    REQUIRE_EQ(deserialized.size(), 1);

    const auto deserialized_jobject = required<object>(deserialized.get("object_value"));

    REQUIRE_EQ(deserialized_jobject.size(), 3);
    REQUIRE_EQ(required<double>(deserialized_jobject.get("number_value")), 4.19);
    REQUIRE_EQ(required<bool>(deserialized_jobject.get("bool_value")), true);
    REQUIRE_EQ(required<string>(deserialized_jobject.get("string_value")), "Hello, world");
}

TEST_CASE("json.Json can be serialized into file properly")
{
    using namespace json;
    using namespace utils;
    namespace files = std::filesystem;

    object jobject {};
    jobject.set("number_value", 4.19);
    jobject.set("bool_value", true);
    jobject.set("string_value", "Hello, world");

    const files::path output_filepath = files::temp_directory_path() / (generate_uuid() + ".png");
    required(serialize_to_file(jobject, output_filepath));

    const auto deserialized = required<object>(required(deserialize_from_file(output_filepath)));

    REQUIRE_EQ(deserialized.size(), 3);
    REQUIRE_EQ(required<double>(deserialized.get("number_value")), 4.19);
    REQUIRE_EQ(required<bool>(deserialized.get("bool_value")), true);
    REQUIRE_EQ(required<string>(deserialized.get("string_value")), "Hello, world");
}
