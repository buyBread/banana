#include <windows.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "banana/core.hh"
#include "banana/logging.hh"
#include "banana/hooks/manager.hh"
#include "banana/imgui/imgui.hh"

using namespace banana;

void imgui::initialize() {    
    if (!ImGui::GetCurrentContext())
        ctx = ImGui::CreateContext();

    ImGui_ImplDX9_Init  (banana::store::d3d9_device);
    ImGui_ImplWin32_Init(store::handle_window);
    
    io    = &ImGui::GetIO();
    style = &ImGui::GetStyle();

    io->FontDefault = io->Fonts->AddFontDefaultVector();

    // imgui::style->AntiAliasedLines       = false;
    // imgui::style->AntiAliasedLinesUseTex = false;
    // imgui::style->AntiAliasedFill        = false;

    store::original_WndProc = (WNDPROC)SetWindowLongPtrA
        (store::handle_window, GWLP_WNDPROC, (LONG_PTR)imgui::WndProc);

    banana::hook_manager.install("imgui");
    
    banana::log.msg("ImGui: initialized");
}

void imgui::shutdown() {
    SetWindowLongPtrA(store::handle_window,
                      GWLP_WNDPROC,
                      (LONG_PTR)store::original_WndProc);

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
            
    ImGui::DestroyContext();

    ctx   = nullptr;
    io    = nullptr;
    style = nullptr;

    store::original_WndProc = nullptr;

    banana::hook_manager.uninstall("imgui");
    
    banana::log.msg("ImGui: shutdown complete");
}