#include "../base.hh"

#include "../../banana.hh"

class hk_Present : public c_hook<hk_Present, hk_fn_sig::Present_t> {

public:
    hk_Present() : c_hook([]() -> void* { return gimmie_virt(
        banana::store::d3d9_device,
        banana::vtables::IDirect3DDevice9::Present);
    }) {}

    static HRESULT WINAPI detour(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
        // DEBUG_HOOK_THREAD;

        return m_original(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }
}; hk_Present hk {};