#pragma once

namespace imgui
{
    void imgui_assert_handler(const char* expr, const char* file, int line);
}

#define IM_ASSERT(_EXPR) \
    do { \
        if (!(_EXPR)) { \
        imgui::imgui_assert_handler(#_EXPR, __FILE__, __LINE__); \
        } \
    } while (0)
