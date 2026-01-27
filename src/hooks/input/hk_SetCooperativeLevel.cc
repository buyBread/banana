#include "../base.hh"

#include "../../banana.hh"
#include "../../util.hh"

class hk_SetCooperativeLevel : public c_hook<hk_SetCooperativeLevel, banana::hk_fn_sig::SetCooperativeLevel_t> {
    
public:
    hk_SetCooperativeLevel() : c_hook([]() -> void* { return gimmie_virt_dinput8(banana::vtables::IDirectInputDeviceA::SetCooperativeLevel); }) {}

    static HRESULT WINAPI detour(IDirectInputDevice8* pDevice, HWND hwnd, DWORD dwFlags) {
        // otherwise, prevents ImGui from processing inputs
        dwFlags &= ~DISCL_EXCLUSIVE;
        dwFlags |= DISCL_NONEXCLUSIVE;

        return m_original(pDevice, hwnd, dwFlags);
    }
}; hk_SetCooperativeLevel hk {};