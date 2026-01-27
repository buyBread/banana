#include "../base.hh"

/*
    create script?
*/

class hk_sub_759EB0 : public c_hook<hk_sub_759EB0, hk_fn_sig::sub_759EB0_t> {

public:

    hk_sub_759EB0() : c_hook((void*)0x00759EB0) {}

    static void* __cdecl detour(resource_descriptor_t* descriptor) {
        return m_original(descriptor);
    }
};

// hk_sub_759EB0 hk {};