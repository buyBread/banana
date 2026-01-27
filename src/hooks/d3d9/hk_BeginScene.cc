#include "../base.hh"

#include "../../banana.hh"

class hk_BeginScene : public c_hook<hk_BeginScene, banana::hk_fn_sig::BeginScene_t> {

public:
    hk_BeginScene() : c_hook([]() -> void* { return gimmie_virt(
        banana::store::d3d9_device,
        banana::vtables::IDirect3DDevice9::BeginScene);
    }) {}

    static HRESULT WINAPI detour(IDirect3DDevice9* pDevice) {
        // DEBUG_HOOK_THREAD;

        if (_ReturnAddress() == (void*)0x9e8e12)
            return m_original(pDevice);

        return m_original(pDevice);
    }
}; hk_BeginScene hk {};