
export module json:serialization;

import std;
import utilities;

import :types;
import :common;

namespace json
{
    export [[nodiscard]] result<document> deserialize(std::span<const std::uint8_t> data);
    export [[nodiscard]] result<std::vector<std::uint8_t>> serialize(const document& doc);

    export [[nodiscard]] result<document> deserialize(const std::string_view data)
    {
        return deserialize(std::span { reinterpret_cast<const std::uint8_t*>(data.data()), data.length() });
    }

    export [[nodiscard]] result<document> deserialize_from_file(const std::filesystem::path& file)
    {
        return utils::read_binary_file(file)
            .and_then([] (const auto& data) { return deserialize(data); });
    }

    export [[nodiscard]] outcome serialize_to_file(const document& doc, const std::filesystem::path& file)
    {
        return serialize(doc)
            .and_then([&file] (const auto& data) { return utils::write_binary_file(data, file); });
    }
}