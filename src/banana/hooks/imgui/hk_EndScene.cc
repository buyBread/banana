#include "banana/hooks/base.hh"
#include "banana/core.hh"
#include "banana/imgui/manager.hh"
#include "util/gimmie/virt.hh"
#include "util/vtables/IDirect3DDevice9.hh"
#include "util/macros/lambda.hh"

DEFINE_HOOK(hk_EndScene, hook_signatures::imgui::EndScene) {

public:
    CONSTRUCT_HOOK(hk_EndScene,
        LAMBDA {
            return util::gimmie::virt(store::d3d9_device, util::vtables::IDirect3DDevice9::EndScene);
        },
        "imgui") {}

    static HRESULT WINAPI detour(IDirect3DDevice9* pDevice) {
        if (_ReturnAddress() == (void*)0x9e8e5f)
            /*
                that's a second EndScene call;
                you cannot draw to it...
            */
            return m_original(pDevice);

        imgui::gui_manager.run();

        return m_original(pDevice);
    }
};

CREATE_HOOK(hk_EndScene);