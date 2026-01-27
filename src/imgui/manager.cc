#include "manager.hh"
#include "imgui.h"
#include "imgui.hh"

#include <d3d9.h>

#include "../banana.hh"

void set_imgui_point_filter(const ImDrawList*, const ImDrawCmd*) {
    IDirect3DDevice9* device = banana::store::d3d9_device;

    if (!device)
        return;

    device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
}

void s_gui_manager::register_GUI(i_gui* GUI) {
    this->m_GUIs.push_back(GUI);
}

void s_gui_manager::render() {
    if (!banana::safe)
        return;

    if (!IMGUI::initialized)
        return;

    if (!IMGUI::io)
        return;

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::GetBackgroundDrawList()->AddCallback(
        set_imgui_point_filter,
        nullptr);

    for (auto &GUI : this->m_GUIs) {
        if (!GUI->logic())
            continue;

        if (IMGUI::render)
            GUI->draw();
    }

    ImGui::EndFrame();
    
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    IMGUI::io->MouseDrawCursor = IMGUI::render;
}
