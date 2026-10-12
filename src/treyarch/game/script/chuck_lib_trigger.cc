#include <cstring>

#include "retail.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/script/chuck_lib_trigger.hh"
#include "treyarch/game/trigger_manager.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_83E040
u32 slc_trigger_t::find_instance(const mash::string &name) const {
    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* trigger = (arch_base*)retail::sub_7456D0((u32*)trigger_manager::inst(), &hash.source_hash_code); // trigger_manager::find_instance

    return trigger->my_handle.value;
}

bool slf__find_trigger__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855080)(this, stack);
}

bool slf__find_trigger__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855100)(this, stack);
}

bool slf__trigger_exists__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855160)(this, stack);
}

bool slf__trigger_exists__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8551D0)(this, stack);
}

bool slf__create_trigger__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855250)(this, stack);
}

bool slf__create_trigger__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855310)(this, stack);
}

bool slf__create_trigger__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8553F0)(this, stack);
}

bool slf__create_trigger__str__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8554E0)(this, stack);
}

bool slf__create_double_sphere_trigger__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8555D0)(this, stack);
}

bool slf__create_box_trigger_from_frame__vector3d__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8556C0)(this, stack);
}

bool slf__create_box_trigger__vector3d__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8557A0)(this, stack);
}

bool slf__create_box_trigger__str__vector3d__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855AA0)(this, stack);
}

bool slf__destroy_trigger__trigger__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855DC0)(this, stack);
}

bool slf__trigger_is_valid__trigger__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855E00)(this, stack);
}

bool slf__trigger__get_abs_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854AC0)(this, stack);
}

bool slf__trigger__get_triggered_entity_list__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880C00)(this, stack);
}

bool slf__trigger__contains__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854B40)(this, stack);
}

bool slf__trigger__set_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854BD0)(this, stack);
}

bool slf__trigger__set_use_any_char__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854C20)(this, stack);
}

bool slf__trigger__set_multiple_entrance__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854C70)(this, stack);
}

bool slf__trigger__set_sees_dead_people__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854CC0)(this, stack);
}

bool slf__trigger__set_uses_extended_events__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854D10)(this, stack);
}

bool slf__trigger__set_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854D60)(this, stack);
}

bool slf__trigger__get_radius__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854DB0)(this, stack);
}

bool slf__trigger__set_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854E10)(this, stack);
}

bool slf__trigger__is_box_trigger__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854E50)(this, stack);
}

bool slf__trigger__is_sphere_trigger__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854ED0)(this, stack);
}

bool slf__trigger__is_occupied__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854F50)(this, stack);
}

bool slf__trigger__get_box_trigger_frame__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880C70)(this, stack);
}

bool slf__trigger__set_box_trigger_frame__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854FD0)(this, stack);
}
