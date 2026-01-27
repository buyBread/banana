#include "../base.hh"

#include "../../banana.hh"
#include "../../imgui/manager.hh"

class hk_EndScene : public c_hook<hk_EndScene, banana::hk_fn_sig::EndScene_t> {

public:
    hk_EndScene() : c_hook([]() -> void* { return gimmie_virt(
        banana::store::d3d9_device,
        banana::vtables::IDirect3DDevice9::EndScene);
    }) {}

    static HRESULT WINAPI detour(IDirect3DDevice9* pDevice) {
        // DEBUG_HOOK_THREAD;

        if (_ReturnAddress() == (void*)0x9e8e5f)
            // there's always a later EndScene call @ 0x9e8e5f
            // you cannot draw at that point
            return m_original(pDevice);

        s_gui_manager::get().render();

        return m_original(pDevice);
    }
}; hk_EndScene hk {};