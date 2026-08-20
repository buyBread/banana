#pragma once

#include <windows.h>
#include <imgui.h>

namespace banana { namespace imgui {
    inline bool render = false;

    inline ImGuiContext* ctx   = nullptr;
    inline ImGuiIO*      io    = nullptr;
    inline ImGuiStyle*   style = nullptr;

    extern LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    namespace store {
        inline HWND handle_window = nullptr;

        inline WNDPROC original_WndProc = nullptr;
    } // store
}}

using namespace banana;