#include <cstring>

#include "retail.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/script/chuck_lib_controller.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_83F4A0
u32 slc_script_controller_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF) || std::strncmp(name.c_str(), "CONTROLLER_1", 0xFFFF))
        return 0;

    return references::script_controller.read()->my_handle.value;
}

bool slf__script_controller__get_button_state__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863390)(this, stack);
}

bool slf__script_controller__is_button_pressed__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8633F0)(this, stack);
}

bool slf__script_controller__button_pressed_time__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863450)(this, stack);
}

bool slf__script_controller__was_button_triggered__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8634B0)(this, stack);
}

bool slf__script_controller__are_X_and_O_inverted__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863510)(this, stack);
}

bool slf__script_controller__get_axis_dir__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863550)(this, stack);
}

bool slf__script_controller__get_axis_angle__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8635E0)(this, stack);
}

bool slf__script_controller__get_axis_force__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863630)(this, stack);
}
