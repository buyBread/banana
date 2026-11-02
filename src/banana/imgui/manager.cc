
#include <algorithm>
#include <format>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <d3d9.h>

#include "banana/core.hh"
#include "banana/lifecycle.hh"
#include "banana/util/types.hh"
#include "banana/imgui/manager.hh"
#include "banana/imgui/imgui.hh"

namespace {
    void set_imgui_point_filter(const ImDrawList*, const ImDrawCmd*) {
        banana::store::d3d9_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        banana::store::d3d9_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    }

    void begin_frame() {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::GetBackgroundDrawList()->AddCallback(set_imgui_point_filter, nullptr);
    }

    void end_frame() {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
}

void s_gui_manager::register_GUI(i_gui* gui) {
    if (!gui)
        return;

    if (std::find(m_GUIs.begin(), m_GUIs.end(), gui) != m_GUIs.end())
        return;

    gui->m_id = m_next_id++;

    m_GUIs.push_back(gui);
}

void s_gui_manager::unregister_GUI(i_gui* gui) noexcept {
    std::erase(m_GUIs, gui);
}

i_gui::i_gui(std::string title, bool initially_open) : m_title(std::move(title)),
                                                       m_open(initially_open) {

    s_gui_manager::get().register_GUI(this);
}

i_gui::~i_gui() {
    s_gui_manager::get().unregister_GUI(this);
}

void s_gui_manager::draw_top_bar() {
    if (!ImGui::BeginMainMenuBar()) return;

    ImGui::TextUnformatted("banana");
    ImGui::Separator();

    if (ImGui::BeginMenu("windows")) {
        if (ImGui::MenuItem("restore all")) {
            for (auto* gui : m_GUIs)
                gui->open();
        }

        if (ImGui::MenuItem("minimize all")) {
            for (auto* gui : m_GUIs)
                gui->minimize();
        }

        if (ImGui::MenuItem("close all")) {
            for (auto* gui : m_GUIs)
                gui->close();
        }

        ImGui::Separator();
        
        for (auto* gui : m_GUIs) {
            ImGui::PushID(gui);
            if (ImGui::BeginMenu(gui->m_title.c_str())) {
                if (ImGui::MenuItem("open / restore", nullptr, false, !gui->m_open || gui->m_minimized))
                    gui->open();

                if (ImGui::MenuItem("minimize", nullptr, false, gui->m_open && !gui->m_minimized))
                    gui->minimize();

                if (ImGui::MenuItem("close", nullptr, false, gui->m_open))
                    gui->close();

                ImGui::EndMenu();
            }

            ImGui::PopID();
        }

        ImGui::EndMenu();
    }

    for (auto* gui : m_GUIs) {
        ImGui::SameLine(0.0f, 6.0f);
        ImGui::PushID(gui);

        const char* prefix = !gui->m_open ? "+ " : (gui->m_minimized ? "_ " : "* ");
        
        const auto label = std::format("{}{}", prefix, gui->m_title);
        
        if (ImGui::SmallButton(label.c_str())) {
            if (!gui->m_open || gui->m_minimized)
                gui->open();
            else
                gui->minimize();
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Middle)) gui->close();

        if (ImGui::BeginPopupContextItem("panel-actions")) {
            if (ImGui::MenuItem("open / restore"))
                gui->open();

            if (ImGui::MenuItem("minimize", nullptr, false, gui->m_open))
                gui->minimize();

            if (ImGui::MenuItem("close", nullptr, false, gui->m_open))
                gui->close();
            
            ImGui::EndPopup();
        }

        if (gui->m_open) {
            ImGui::SameLine(0.0f, 2.0f);
            
            if (ImGui::SmallButton("_"))
                gui->minimize();
            
            ImGui::SameLine(0.0f, 2.0f);
            
            if (ImGui::SmallButton("x"))
                gui->close();
        }

        ImGui::PopID();
    }

    ImGui::EndMainMenuBar();
}

void s_gui_manager::run() {
    if (banana::state::current() != e_lifecycle::ready || !banana::imgui::ctx)
        return;

    for (auto* gui : m_GUIs)
        gui->m_logic_visible = gui->logic();

    if (!banana::imgui::render) {
        if (banana::imgui::io)
            banana::imgui::io->MouseDrawCursor = false;
        
        return;
    }

    begin_frame();
    draw_top_bar();

    for (auto* gui : m_GUIs) {
        if (!gui->m_open || gui->m_minimized || !gui->m_logic_visible)
            continue;

        const auto name = std::format("{}###banana.panel.{}", gui->m_title, gui->m_id);

        ImGui::SetNextWindowSize(ImVec2(860.0f, 480.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(45.0f + 24.0f * (gui->m_id % 8),
                                       65.0f + 20.0f * (gui->m_id % 8)), ImGuiCond_FirstUseEver);
        if (gui->m_focus) {
            ImGui::SetNextWindowFocus();
            gui->m_focus = false;
        }

        bool still_open = gui->m_open;

        if (ImGui::Begin(name.c_str(), &still_open, ImGuiWindowFlags_NoCollapse))
            gui->draw();
        
        ImGui::End();

        if (!still_open)
            gui->close();
    }

    end_frame();

    if (banana::imgui::io)
        banana::imgui::io->MouseDrawCursor = true;
}
