#include "../base.hh"

/*
    ???
*/

class hk_sub_A6C7E0 : public c_hook<hk_sub_A6C7E0, hk_fn_sig::sub_A6C7E0_t> {
    
public:

    hk_sub_A6C7E0() : c_hook((void*)0x00A6C7E0) {}

    static void __thiscall detour(uint32_t* out, bool skip, const char* string, uint32_t hash) {
        if (skip)
            return;

        if (hash)
            *out = hash;
        else if (string && *string) {
            static auto detour_sub_408DD0 = s_hook_manager::get()
                .get_detour<hk_fn_sig::sub_408DD0_t>("sub_408DD0");

            *out = detour_sub_408DD0(string);
        } else
            *out = 0;

        // m_original(ecx, edx, arg_0, arg_4, arg_8);
    }

}; hk_sub_A6C7E0 hk {};