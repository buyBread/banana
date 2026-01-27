#include "../base.hh"

/*
    nglProfilerFramerate
*/

class hk_sub_9E9AE0 : public c_hook<hk_sub_9E9AE0, hk_fn_sig::sub_9E9AE0_t> {

public:

    hk_sub_9E9AE0() : c_hook((void*)0x009E9AE0) {}

    static void __cdecl detour() {
        return m_original();
    }

}; hk_sub_9E9AE0 hk {};