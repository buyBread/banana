#include "../base.hh"

/*
    script_instance thread scheduler
*/

class hk_sub_A1E980 : public c_hook<hk_sub_A1E980, hk_fn_sig::sub_A1E980_t> {

public:

    hk_sub_A1E980() : c_hook((void*)0x00A1E980) {}

    static void* __thiscall detour(void* instance, char mode) {
        return m_original(instance, mode);
    }

};

// hk_sub_A1E980 hk {};