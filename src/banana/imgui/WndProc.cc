#include "banana/util/types.hh"
#include "banana/imgui/imgui.hh"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK banana::imgui::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
        // ignore Windblows key-repeat and don't leak the toggle to the game
        if ((static_cast<u64>(wParam) & (1ull << 30)) == 0)
            render = !render;

        return TRUE;
    }

    if (uMsg == WM_KEYUP && wParam == VK_INSERT)
        return TRUE;

    if (render && ctx) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
            return TRUE;

        if (io) {
            const bool mouse    = uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST;
            const bool keyboard = uMsg >= WM_KEYFIRST   && uMsg <= WM_KEYLAST;

            if ((mouse && io->WantCaptureMouse) || (keyboard && io->WantCaptureKeyboard))
                return TRUE;
        }
    }

    return CallWindowProc(store::original_WndProc, hwnd, uMsg, wParam, lParam);
}