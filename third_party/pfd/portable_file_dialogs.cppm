module;
#include "portable_file_dialogs.h"

export module pfd;
export import std;

namespace pfd
{
    export struct filter
    {
        std::string_view name;
        std::string_view regex;
    };

    export constexpr filter all_files = { "All Files", "*" };

    static std::vector<std::string> to_pfd_filters(const std::span<const filter> filters)
    {
        return filters
            | std::views::transform([] (auto& f) { return std::array{ f.name, f.regex }; })
            | std::views::join
            | std::ranges::to<std::vector<std::string>>();
    }

    // TODO: asynchronous
    export [[nodiscard]] std::optional<std::filesystem::path> open_file(
        const std::string_view title,
        const std::optional<std::filesystem::path>& path = std::nullopt,
        const std::optional<std::span<filter>>& filters = std::nullopt)
    {
        const auto pfd_path = path.transform([] (auto& p) { return p.string(); }).value_or({});
        const auto pfd_filters = filters.transform(to_pfd_filters).value_or({});
        const auto result = open_file::open_file(title.data(), pfd_path, pfd_filters, opt::none).result();

        return result.empty() ? std::nullopt : std::make_optional(result[0]);
    }

    export [[nodiscard]] button message_dialog(
        const std::string_view title,
        const std::string_view text,
        const choice buttons,
        const icon icon)
    {
        return message(title.data(), text.data(), buttons, icon).result();
    }

    export void error_dialog(const std::string_view title, const std::string_view text)
    {
        std::ignore = message_dialog(title, text, choice::ok, icon::error);
    }

}