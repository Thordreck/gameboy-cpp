module;
#include "imgui.h"

export module imgui:widgets;
export import std;

export import :common;

namespace imgui
{
    export vec2 get_available_content_space()
    {
        const ImVec2 size = ImGui::GetContentRegionAvail();
        return { size.x, size.y };
    }

    using close_widget_fn_t = void(*)();

    export class scoped_widget
    {
    public:
        ~scoped_widget()
        {
            if (close_widget_fn != nullptr)
            {
                close_widget_fn();
                close_widget_fn = nullptr;
            }
        }

        scoped_widget(const scoped_widget&) = delete;
        scoped_widget& operator=(const scoped_widget&) = delete;

        scoped_widget(scoped_widget&& other) noexcept
            : scoped_widget { std::exchange(other.close_widget_fn, nullptr) }
        {}

        scoped_widget& operator=(scoped_widget&& other) noexcept
        {
            close_widget_fn = std::exchange(other.close_widget_fn, nullptr);
            return *this;
        }

    public:
        explicit scoped_widget(close_widget_fn_t close_fn)
            : close_widget_fn(close_fn)
        {}

        close_widget_fn_t close_widget_fn;
    };

    export std::optional<scoped_widget> main_menu_bar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            return scoped_widget(&ImGui::EndMainMenuBar);
        }

        return std::nullopt;
    }

    export std::optional<scoped_widget> menu(const std::string_view label)
    {
        if (ImGui::BeginMenu(label.data()))
        {
            return scoped_widget(&ImGui::EndMenu);
        }

        return std::nullopt;
    }

    export bool menu_item(const std::string_view label)
    {
        return ImGui::MenuItem(label.data());
    }

    export std::optional<scoped_widget> window(const std::string_view name)
    {
        if (ImGui::Begin(name.data()))
        {
            return scoped_widget(&ImGui::End);
        }

        return std::nullopt;
    }

    export void image(const texture_id id, const vec2 size)
    {
        ImGui::Image(id, { size.x, size.y });
    }

}