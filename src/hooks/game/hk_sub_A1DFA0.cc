#include "../base.hh"

/*
    script_instance::add_thread
*/

class hk_sub_A1DFA0 : public c_hook<hk_sub_A1DFA0, hk_fn_sig::sub_A1DFA0_t> {

public:

    hk_sub_A1DFA0() : c_hook((void*)0x00A1DFA0) {}

    static vm_thread* __thiscall detour(script_instance* instance, vm_executable* executable, void* context, int stack_size) {
        return m_original(instance, executable, context, stack_size);
    }
};

hk_sub_A1DFA0 hk {};