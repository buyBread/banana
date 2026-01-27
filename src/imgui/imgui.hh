#pragma once

#include <atomic>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#define IMGUI     banana::imgui
#define IMGUI_EXT IMGUI::ext

namespace banana {

namespace imgui {
    inline std::atomic<bool> initialized = false;
    inline std::atomic<bool> render      = false;

    inline ImGuiIO*    io    = nullptr;
    inline ImGuiStyle* style = nullptr;
} // imgui

} // banana