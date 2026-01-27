#include "../base.hh"

/*
    nglHash
*/

class hk_sub_408DD0 : public c_hook<hk_sub_408DD0, banana::hk_fn_sig::sub_408DD0_t> {
    
public:

    hk_sub_408DD0() : c_hook((void*)0x00408DD0) {}

    static uint32_t __cdecl detour(const char* string) {
        uint32_t hash = 0;

        for (uint8_t c = static_cast<uint8_t>(*string); c; c = static_cast<uint8_t>(*++string)) {
            const uint32_t x = c - 'A';

            hash = hash * 33u + static_cast<uint8_t>(c + ((x <= ('Z' - 'A')) << 5));
        }

        return hash;

        // return m_original(arg_0);
    }

}; hk_sub_408DD0 hk {};