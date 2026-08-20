#include "banana/imgui/imgui.hh"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace banana { namespace imgui {
    LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
            return TRUE;

        if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
            render = !render;

        if (render) {
            // see: "./banana/hooks/input/"
            if ((uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST && io->WantCaptureMouse) ||
                (uMsg >= WM_KEYFIRST   && uMsg <= WM_KEYLAST   && io->WantCaptureKeyboard))

                return TRUE;
        }

        return CallWindowProc(store::original_WndProc, hwnd, uMsg, wParam, lParam);
    }
}} // banana::imgui