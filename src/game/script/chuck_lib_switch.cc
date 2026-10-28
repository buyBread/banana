#include "retail.hh"
#include "game/script/chuck_lib_switch.hh"
#include "game/wds/entity/entity.hh"

using namespace treyarch;

// sub_8721A0
bool slf__entity__create_switch_obj_interface__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_entity() && !self->has_ifc(entity_ifc_switch_obj))
        retail::sub_644DB0((i32*)self); // entity_base::create_switch_obj_ifc

    return true;
}

// sub_8721E0
bool slf__entity__switch_set_no_use__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_no_use();

    return true;
}

// sub_872220
bool slf__entity__switch_set_usable__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_usable();

    return true;
}

// sub_872260
bool slf__entity__switch_get_state__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->has_ifc(entity_ifc_switch_obj) ? (f32)self->switch_obj_ifc()->state : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8722D0
bool slf__entity__switch_set_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_enabled(on != 0.0f);

    return true;
}

// sub_872330
bool slf__entity__switch_set_state__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_state(on != 0.0f ? switch_obj_interface::switch_on : switch_obj_interface::switch_off);

    return true;
}

// sub_872390
bool slf__entity__switch_set_web_targetable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_web_targetable(on != 0.0f);

    return true;
}

// sub_8723F0
bool slf__entity__switch_set_web_target_only__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_web_target_only(on != 0.0f);

    return true;
}

// sub_872450
bool slf__entity__switch_set_alarm__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_alarm(on != 0.0f);

    return true;
}

// sub_8724B0
bool slf__entity__switch_set_single_use__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_single_use(on != 0.0f);

    return true;
}

// sub_872510
bool slf__entity__switch_set_ignore_hero__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->set_ignore_hero(on != 0.0f);

    return true;
}

// sub_872570
bool slf__entity__switch_set_flick_animation__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto anim_name = (const char*)stack.local_uint(0x04);

    if (!self || !self->has_ifc(entity_ifc_switch_obj))
        return true;

    string_hash animation;
    animation.initialize(mash::ALLOCATED, anim_name);

    self->switch_obj_ifc()->flick_animation = animation;

    return true;
}

// sub_8725D0
bool slf__entity__switch_set_radius_mod__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto radius_mod = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_switch_obj))
        self->switch_obj_ifc()->radius_mod = radius_mod;

    return true;
}
