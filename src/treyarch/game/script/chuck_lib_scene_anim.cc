#include "retail.hh"
#include "treyarch/game/script/chuck_lib_scene_anim.hh"

using namespace treyarch;

bool slf__create_cut_scene__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876390)(this, stack);
}

bool slf__create_preregistered_cut_scene__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876490)(this, stack);
}

bool slf__wait_play__cut_scene__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876590)(this, stack);
}

bool slf__stop_cut_scene__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824EA0)(this, stack);
}

bool slf__wait_run_toa_cutscene__dynamic_array__dynamic_array__dynamic_array__dynamic_array__dynamic_array__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876680)(this, stack);
}
