#include <d3d9.h>
#include <imgui_impl_dx9.h>

#include "banana/hooks/base.hh"
#include "util/gimmie/virt.hh"
#include "util/vtables/IDirect3DDevice9.hh"
#include "util/macros/lambda.hh"

DEFINE_HOOK(hk_Reset, hook_signatures::device_lifecycle::Reset) {
    
public:
    CONSTRUCT_HOOK(hk_Reset,
        LAMBDA {
            return util::gimmie::virt(store::d3d9_device, util::vtables::IDirect3DDevice9::Reset);
        },
        "device_lifecycle") {}

    static HRESULT WINAPI detour(IDirect3DDevice9*      pDevice,
                                 D3DPRESENT_PARAMETERS* pPresentationParameters) {

        banana::state::update(e_lifecycle::rebuilding);

        ImGui_ImplDX9_InvalidateDeviceObjects();

        HRESULT result = m_original(pDevice, pPresentationParameters);

        if (SUCCEEDED(result)) {
            ImGui_ImplDX9_CreateDeviceObjects();

            banana::state::update(e_lifecycle::ready);
        }

        return result;
    }
};

CREATE_HOOK(hk_Reset);