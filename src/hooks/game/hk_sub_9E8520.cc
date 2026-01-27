#include "../base.hh"

/*
    ???
*/

class hk_sub_9E8520 : public c_hook<hk_sub_9E8520, banana::hk_fn_sig::sub_9E8520_t> {
    
public:

    hk_sub_9E8520() : c_hook((void*)0x009E8520) {}

    static void* __fastcall detour(void* ecx, void* edx) {
        return m_original(ecx, edx);
    }

}; hk_sub_9E8520 hk {};