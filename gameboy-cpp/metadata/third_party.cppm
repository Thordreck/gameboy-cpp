export module metadata:third_party;

import std;
import json;

import :common;
import :third_party_metadata;

namespace meta
{
    export struct third_party_module_metadata
    {
        std::string name;
        std::string url;
        std::string license;
        std::optional<std::string> notes;
    };

    [[nodiscard]] result<third_party_module_metadata> deserialize_metadata(const json::object& jobject)
    {
        using namespace json;
        third_party_module_metadata entry {};

        return get(jobject, "name")
            .and_then([] (auto&& field) { return as<std::string>(field); })
            .transform([&entry] (auto&& name){ entry.name = name; })
            .and_then([&jobject] { return get(jobject, "url"); })
            .and_then([] (auto&& field) { return as<std::string>(field); })
            .transform([&entry] (auto&& url){ entry.url = url; })
            .and_then([&jobject] { return get(jobject, "license"); })
            .and_then([] (auto&& field) { return as<std::string>(field); })
            .transform([&entry] (auto&& license){ entry.license = license; })
            .transform([&entry] { return entry; });
    }

    [[nodiscard]] result<std::vector<third_party_module_metadata>> deserialize_metadata(const json::array& jarray)
    {
        using namespace json;

        std::vector<third_party_module_metadata> results {};

        for (size_t i = 0; i < jarray.size(); i++)
        {
            const auto deserialization = as<object>(jarray.get(i))
                .and_then([] (auto&& element) { return deserialize_metadata(element); });

            if (!deserialization)
            {
                return std::unexpected { deserialization.error() };
            }

            results.push_back(deserialization.value());
        }

        return results;
    }

    export [[nodiscard]] result<std::vector<third_party_module_metadata>> get_third_party_metadata()
    {
        using namespace json;

        return deserialize(third_party_metadata)
            .and_then([] (auto&& deserialized) { return as<object>(deserialized); })
            .and_then([] (auto&& jobject) { return get(jobject, "modules"); })
            .and_then([] (auto&& jvalue) { return as<array>(jvalue); })
            .and_then([] (auto&& jarray) { return deserialize_metadata(jarray); });
    }
}
