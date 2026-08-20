#include <d3d9.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <cassert>

#include "banana/hooks/base.hh"
#include "banana/imgui/imgui.hh"
#include "util/gimmie/virt.hh"
#include "util/vtables/IDirect3D9.hh"
#include "util/macros/lambda.hh"

DEFINE_HOOK(hk_CreateDevice, hook_signatures::device_lifecycle::CreateDevice) {

public:
    CONSTRUCT_HOOK(hk_CreateDevice,
        LAMBDA {
            return util::gimmie::virt(store::d3d9, util::vtables::IDirect3D9::CreateDevice);
        },
        "device_lifecycle") {}

    static HRESULT WINAPI detour(IDirect3D9*            pD3D,
                                 UINT                   Adapter,
                                 D3DDEVTYPE             DeviceType,
                                 HWND                   hFocusWindow,
                                 DWORD                  BehaviorFlags,
                                 D3DPRESENT_PARAMETERS* pPresentationParameters,
                                 IDirect3DDevice9**     ppReturnedDeviceInterface) {

        HRESULT result = m_original(pD3D, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

        if (!(SUCCEEDED(result) && ppReturnedDeviceInterface && *ppReturnedDeviceInterface))
            return result;

        /*
            safeguard
            a bit paranoid, but it might happen
        */
        if (banana::store::d3d9_device != nullptr &&
            banana::store::d3d9_device != *ppReturnedDeviceInterface) {

            banana::state::update(e_lifecycle::rebuilding);

            banana::hook_manager.uninstall("imgui");
            banana::hook_manager.uninstall("device_lifecycle", "Reset");

            SetWindowLongPtrA(
                imgui::store::handle_window,
                GWLP_WNDPROC,
                (LONG_PTR)imgui::store::original_WndProc
            );

            ImGui_ImplDX9_Shutdown();
            ImGui_ImplWin32_Shutdown();
            
            ImGui::DestroyContext();
            imgui::ctx = nullptr;

            banana::log.dbg("game rebuilt IDirect3DDevice9");

            imgui::store::original_WndProc = nullptr;
        }

        if (!ImGui::GetCurrentContext())
            imgui::ctx = ImGui::CreateContext();

        banana::store::d3d9_device = *ppReturnedDeviceInterface;

        banana::log.dbg("stored IDirect3DDevice9");

        imgui::store::handle_window = pPresentationParameters->hDeviceWindow ?
            pPresentationParameters->hDeviceWindow : hFocusWindow;

        banana::log.dbg("stored HWND");

        // both want store::d3d9_device
        banana::hook_manager.enable_hook("device_lifecycle", "Reset");
        banana::hook_manager.install("imgui");

        ImGui_ImplDX9_Init  (banana::store::d3d9_device);
        ImGui_ImplWin32_Init(imgui::store::handle_window);

        imgui::io    = &ImGui::GetIO();
        imgui::style = &ImGui::GetStyle();

        imgui::io->FontDefault = banana::imgui::io->Fonts->AddFontDefaultVector();

        // imgui::style->AntiAliasedLines       = false;
        // imgui::style->AntiAliasedLinesUseTex = false;
        // imgui::style->AntiAliasedFill        = false;

        imgui::store::original_WndProc = (WNDPROC)SetWindowLongPtrA(
            imgui::store::handle_window, 
            GWLP_WNDPROC, 
            (LONG_PTR)imgui::WndProc
        );

        banana::log.msg("ImGui initialized");

        banana::state::update(e_lifecycle::ready);

        return result;
    }
};

CREATE_HOOK(hk_CreateDevice);