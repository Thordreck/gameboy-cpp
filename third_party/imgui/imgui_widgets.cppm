module;
#include <optional>

#include "imgui.h"

export module imgui:widgets;
export import std;

import :common;
import :viewport;

namespace imgui
{
    export enum class window_flags : std::uint32_t
    {
        none = ImGuiWindowFlags_None,
        no_title_bar = ImGuiWindowFlags_NoTitleBar,
        no_resize = ImGuiWindowFlags_NoResize,
        no_move = ImGuiWindowFlags_NoMove,
        no_scrollbar = ImGuiWindowFlags_NoScrollbar,
        no_scroll_with_mouse = ImGuiWindowFlags_NoScrollWithMouse,
        no_collapse = ImGuiWindowFlags_NoCollapse,
        always_auto_resize = ImGuiWindowFlags_AlwaysAutoResize,
        no_background  =ImGuiWindowFlags_NoBackground,
        no_saved_settings = ImGuiWindowFlags_NoSavedSettings,
        no_mouse_inputs = ImGuiWindowFlags_NoMouseInputs,
        menu_bar = ImGuiWindowFlags_MenuBar,
        horizontal_scrollbar = ImGuiWindowFlags_HorizontalScrollbar,
        no_focus_on_appearing = ImGuiWindowFlags_NoFocusOnAppearing,
        no_bring_to_front_on_focus = ImGuiWindowFlags_NoBringToFrontOnFocus,
        always_vertical_scrollbar = ImGuiWindowFlags_AlwaysVerticalScrollbar,
        always_horizontal_scrollbar = ImGuiWindowFlags_AlwaysHorizontalScrollbar,
        no_nav_inputs = ImGuiWindowFlags_NoNavInputs,
        no_nav_focus = ImGuiWindowFlags_NoNavFocus,
        unsaved_document = ImGuiWindowFlags_UnsavedDocument,
        no_nav = ImGuiWindowFlags_NoNav,
        no_decoration = ImGuiWindowFlags_NoDecoration,
        no_inputs = ImGuiWindowFlags_NoInputs,
    };

    export vec2 get_available_content_space()
    {
        const ImVec2 size = ImGui::GetContentRegionAvail();
        return { size.x, size.y };
    }

    export vec2 get_display_size()
    {
        const ImVec2 size = ImGui::GetIO().DisplaySize;
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

    export std::optional<scoped_widget> window(const std::string_view name, const window_flags flags = window_flags::none)
    {
        if (ImGui::Begin(name.data(), nullptr, std::to_underlying(flags)))
        {
            return scoped_widget(&ImGui::End);
        }

        return std::nullopt;
    }

    export void image(const texture_id id, const vec2 size)
    {
        ImGui::Image(id, { size.x, size.y });
    }

    export void set_next_window_position(const vec2& pos)
    {
        ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    }

    export void set_next_window_size(const vec2& size)
    {
        ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    }


}