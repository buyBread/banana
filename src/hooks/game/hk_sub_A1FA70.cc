#include "../base.hh"

/*
    link script
*/

class hk_sub_A1FA70 : public c_hook<hk_sub_A1FA70, hk_fn_sig::sub_A1FA70_t> {

public:

    hk_sub_A1FA70() : c_hook((void*)0x00A1FA70) {}

    static void __thiscall detour(void* root) {
        m_original(root);
    }
};

// hk_sub_A1FA70 hk {};