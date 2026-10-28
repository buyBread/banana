#include <cstring>

#include "retail.hh"
#include "game/arch_base.hh"
#include "game/game.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/script/chuck_lib_controller.hh"
#include "game/script/script_controller.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_83F4A0
u32 slc_script_controller_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF) || std::strncmp(name.c_str(), "CONTROLLER_1", 0xFFFF))
        return 0;

    return references::script_controller.read()->my_handle.value;
}

// sub_863390
bool slf__script_controller__get_button_state__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (script_controller*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto number = (i32)stack.local_num(0x04);

    f32 result = self && self->enabled ? self->buttons[number].normalized_state() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8633F0
bool slf__script_controller__is_button_pressed__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (script_controller*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto number = (i32)stack.local_num(0x04);

    f32 result = self && self->enabled ? (f32)self->buttons[number].pressed() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_863450
bool slf__script_controller__button_pressed_time__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (script_controller*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto number = (i32)stack.local_num(0x04);

    f32 result = self && self->enabled ? self->buttons[number].get_pressed_time() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8634B0
bool slf__script_controller__was_button_triggered__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (script_controller*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto number = (i32)stack.local_num(0x04);

    f32 result = self && self->enabled && self->buttons[number].triggered() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_863510
bool slf__script_controller__are_X_and_O_inverted__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    arch_base_vhandle(stack.local_uint(0x00)).resolve(); // resolved, then not used

    f32 result = references::x_and_o_inverted.read() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_863550
bool slf__script_controller__get_axis_dir__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (script_controller*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto number = (i32)stack.local_num(0x04);

    // nothing is written for a missing or disabled controller
    vector3d result;

    if (self && self->enabled) {
        const vector2d &dir = self->axes[number].stick_dir.current;

        result.x = dir.x;
        result.y = dir.y;
        result.z = 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8635E0
bool slf__script_controller__get_axis_angle__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (script_controller*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto number = (i32)stack.local_num(0x04);

    f32 result = self && self->enabled ? self->axes[number].stick_angle.current : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_863630
bool slf__script_controller__get_axis_force__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (script_controller*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto number = (i32)stack.local_num(0x04);

    f32 result = self && self->enabled ? self->axes[number].force.current : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}
