#include <d3d9.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <cassert>

#include "banana/core.hh"
#include "banana/lifecycle.hh"
#include "banana/hooks/base.hh"
#include "banana/imgui/imgui.hh"
#include "util/gimmie/virt.hh"
#include "util/vtables/IDirect3D9.hh"
#include "util/macros/lambda.hh"

DEFINE_HOOK(hk_CreateDevice, hook_signatures::device_lifecycle::CreateDevice) {

    static bool has_device_changed(IDirect3DDevice9* returned_device) {
        return !(banana::store::d3d9_device != nullptr &&
                 banana::store::d3d9_device != returned_device);
    }

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
        if (!has_device_changed(*ppReturnedDeviceInterface)) {
            banana::state::update(e_lifecycle::rebuilding);

            banana::hook_manager.uninstall("device_lifecycle", "Reset");

            imgui::shutdown();

            HK_DBG("game rebuilt IDirect3DDevice9");
        }

        banana::store::d3d9_device = *ppReturnedDeviceInterface;

        HK_DBG("stored IDirect3DDevice9");

        imgui::store::handle_window = pPresentationParameters->hDeviceWindow ?
            pPresentationParameters->hDeviceWindow : hFocusWindow;

        HK_DBG("stored HWND");

        // wants both device & window handle
        imgui::initialize();

        banana::hook_manager.enable_hook("device_lifecycle", "Reset");

        banana::state::update(e_lifecycle::ready);

        return result;
    }

    void clean_up() override {
        imgui::shutdown();
    }
};

CREATE_HOOK(hk_CreateDevice);