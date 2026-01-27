#include "../base.hh"

#include "../../ngl/macros.hh"
#include "../../ngl/soap/storage.hh"

class hk_sub_9F0100 : public c_hook<hk_sub_9F0100, hk_fn_sig::sub_9F0100_t> {
    
public:
    hk_sub_9F0100() : c_hook((void*)0x009F0100) {}

    static bool __fastcall detour(void* ecx, void* edx, int mode) {
        NGL_SOAP::storage::instance = ecx;

        return m_original(ecx, edx, mode);
    }

}; hk_sub_9F0100 hk {};