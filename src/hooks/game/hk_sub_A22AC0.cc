#include "../base.hh"

/*
    vm_thread::~vm_thread()
*/

class hk_sub_A22AC0 : public c_hook<hk_sub_A22AC0, hk_fn_sig::sub_A22AC0_t> {

public:

    hk_sub_A22AC0() : c_hook((void*)0x00A22AC0) {}

    static void __thiscall detour(vm_thread* thread) {
        m_original(thread);
    }
};

hk_sub_A22AC0 hk {};