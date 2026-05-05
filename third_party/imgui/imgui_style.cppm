module;
#include <utility>

#include "imgui.h"
#include "imgui_internal.h"

export module imgui:style;

import std;
import :common;

namespace imgui
{
    export enum class style_var : std::uint8_t
    {
        alpha = ImGuiStyleVar_Alpha,
        disabled_alpha = ImGuiStyleVar_DisabledAlpha,
        window_padding = ImGuiStyleVar_WindowPadding,
        window_rounding = ImGuiStyleVar_WindowRounding,
        window_border_size = ImGuiStyleVar_WindowBorderSize,
        window_min_size = ImGuiStyleVar_WindowMinSize,
        window_title_align = ImGuiStyleVar_WindowTitleAlign,
        child_rounding = ImGuiStyleVar_ChildRounding,
        child_border_size = ImGuiStyleVar_ChildBorderSize,
        popup_rounding = ImGuiStyleVar_PopupRounding,
        popup_border_size = ImGuiStyleVar_PopupBorderSize,
        frame_padding = ImGuiStyleVar_FramePadding,
        frame_rounding = ImGuiStyleVar_FrameRounding,
        frame_border_size = ImGuiStyleVar_FrameBorderSize,
        item_spacing = ImGuiStyleVar_ItemSpacing,
        item_inner_spacing = ImGuiStyleVar_ItemInnerSpacing,
        ident_spacing = ImGuiStyleVar_IndentSpacing,
        cell_padding = ImGuiStyleVar_CellPadding,
        scrollbar_size = ImGuiStyleVar_ScrollbarSize,
        scrollbar_rounding = ImGuiStyleVar_ScrollbarRounding,
        scrollbar_padding = ImGuiStyleVar_ScrollbarPadding,
        grab_min_size = ImGuiStyleVar_GrabMinSize,
        grab_rounding = ImGuiStyleVar_GrabRounding,
        image_rounding = ImGuiStyleVar_ImageRounding,
        image_border_size = ImGuiStyleVar_ImageBorderSize,
        tab_rounding = ImGuiStyleVar_TabRounding,
        tab_border_size = ImGuiStyleVar_TabBorderSize,
        tab_min_width_base = ImGuiStyleVar_TabMinWidthBase,
        tab_min_width_shrink = ImGuiStyleVar_TabMinWidthShrink,
        tab_bar_border_size = ImGuiStyleVar_TabBarBorderSize,
        tab_bar_overline_size = ImGuiStyleVar_TabBarOverlineSize,
        table_angle_headers_angle = ImGuiStyleVar_TableAngledHeadersAngle,
        table_angle_headers_text_align = ImGuiStyleVar_TableAngledHeadersTextAlign,
        tree_lines_size = ImGuiStyleVar_TreeLinesSize,
        tree_lines_rounding = ImGuiStyleVar_TreeLinesRounding,
        button_text_align = ImGuiStyleVar_ButtonTextAlign,
        selectable_text_align = ImGuiStyleVar_SelectableTextAlign,
        separator_text_border_size = ImGuiStyleVar_SeparatorTextBorderSize,
        separator_text_align = ImGuiStyleVar_SeparatorTextAlign,
        separator_text_padding = ImGuiStyleVar_SeparatorTextPadding,
    };

    export void set_dark_style()
    {
        ImGui::StyleColorsDark();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    }

    export template <typename T>
    concept StyleVarValue = std::convertible_to<T, float> || std::convertible_to<T, vec2>;

    void push_style_var(const style_var variable, const float value)
    {
        ImGui::PushStyleVar(std::to_underlying(variable), value);
    }

    void push_style_var(const style_var variable, const vec2& value)
    {
        ImGui::PushStyleVar(std::to_underlying(variable), ImVec2(value.x, value.y));
    }

    export void pop_style_var(const int count)
    {
        ImGui::PopStyleVar(count);
    }

    export class scoped_style_var
    {
    public:
        template<StyleVarValue Value>
        static scoped_style_var create(const style_var variable, const Value value)
        {
            push_style_var(variable, value);
            return scoped_style_var{ variable };
        }

        ~scoped_style_var()
        {
            const auto var_info = ImGui::GetStyleVarInfo(std::to_underlying(variable));
            pop_style_var(var_info->Count);
        }

        scoped_style_var(const scoped_style_var&) = delete;
        scoped_style_var& operator=(const scoped_style_var&) = delete;

        /*
        scoped_widget(scoped_widget&& other) noexcept
            : scoped_widget { std::exchange(other.close_widget_fn, nullptr) }
        {}

        scoped_widget& operator=(scoped_widget&& other) noexcept
        {
            close_widget_fn = std::exchange(other.close_widget_fn, nullptr);
            return *this;
        }
        */

    private:
        explicit scoped_style_var(const style_var variable)
            : variable { variable }
        {}

        style_var variable;
    };

}
