#include "../base.hh"

/*
    ???
*/

class hk_sub_9E78A0 : public c_hook<hk_sub_9E78A0, banana::hk_fn_sig::sub_9E78A0_t> {
    
public:

    hk_sub_9E78A0() : c_hook((void*)0x009E78A0) {}

    static void* __fastcall detour(void* ecx, void* edx, void* arg_0) {
        return m_original(ecx, edx, arg_0);
    }

}; hk_sub_9E78A0 hk {};