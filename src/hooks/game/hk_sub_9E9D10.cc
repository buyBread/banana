#include "../base.hh"

/*
    nglDrawProfilers
*/

class hk_sub_9E9D10 : public c_hook<hk_sub_9E9D10, hk_fn_sig::sub_9E9D10_t> {

public:

    hk_sub_9E9D10() : c_hook((void*)0x009E9D10) {}

    static void __cdecl detour() {
        return m_original();
    }

}; hk_sub_9E9D10 hk {};