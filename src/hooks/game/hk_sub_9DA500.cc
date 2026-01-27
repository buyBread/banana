#include "../base.hh"

/*
    nglPresent

    order:
    nglPresent | tid=12708 | caller=0x429bec
    BeginScene | tid=12708 | caller=0x9dcf3a
    EndScene   | tid=12708 | caller=0x9dcf58
    BeginScene | tid=12708 | caller=0x9e8e12
    EndScene   | tid=12708 | caller=0x9e8e5f
    Present    | tid=12708 | caller=0x9e8e7e
*/

class hk_sub_9DA500 : public c_hook<hk_sub_9DA500, banana::hk_fn_sig::sub_9DA500_t> {
    
public:
    hk_sub_9DA500() : c_hook((void*)0x009DA500) {}

    static void __cdecl detour() {
        // DEBUG_HOOK_THREAD;
    
        m_original();
    }

}; hk_sub_9DA500 hk {};