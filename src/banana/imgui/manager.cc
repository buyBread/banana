#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <d3d9.h>

#include "banana/imgui/manager.hh"
#include "banana/imgui/imgui.hh"
#include "banana/core.hh"

#define IMGUI_DEMO 0

void set_imgui_point_filter(const ImDrawList*, const ImDrawCmd*) {
    banana::store::d3d9_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    banana::store::d3d9_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
}

void s_gui_manager::register_GUI(i_gui* GUI) {
    this->m_GUIs.push_back(GUI);
}

void begin_frame() {
#ifndef DEBUG
    if (imgui::render) {
#endif
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::GetBackgroundDrawList()->AddCallback(set_imgui_point_filter, nullptr);
#ifndef DEBUG
    }
#endif
}

void end_frame() {
#ifndef DEBUG
    if (imgui::render) {
#endif
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#ifndef DEBUG
    }
#endif
}

void s_gui_manager::run() {
    if (banana::state::current() != e_lifecycle::ready)
        return;

    begin_frame();

    for (const auto &GUI : this->m_GUIs) {
        if (!GUI->logic())
            continue;

        if (imgui::render)
            GUI->draw();
    }

#if IMGUI_DEMO
    if (imgui::render)
        ImGui::ShowDemoWindow();
#endif

    end_frame();
    
    imgui::io->MouseDrawCursor = imgui::render;
}