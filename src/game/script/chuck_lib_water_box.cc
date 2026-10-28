#include <cstring>

#include "retail.hh"
#include "game/arch_base.hh"
#include "game/script/chuck_lib_water_box.hh"
#include "game/water_box.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_83DAE0
u32 slc_water_box_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    if (!entity)
        return 0;

    return entity->my_handle.value;
}

bool slf__find_water_box__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878070)(this, stack);
}

// sub_877F80
bool slf__water_box__set_rel_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (water_box*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto position = (const vector3d*)&stack.local_uint(0x04);

    if (self)
        retail::sub_626CA0((u32*)self, (u32*)position); // entity_base::set_rel_position

    return true;
}

// sub_877FB0
bool slf__water_box__get_water_height__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (water_box*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto position = (const vector3d*)&stack.local_uint(0x04);

    // without one, the height asked at
    f32 result = self ? ((f32 (__thiscall*)(water_box*, const vector3d*))retail::sub_632AD0)(self, position) : position->y; // water_box::get_water_height

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878010
bool slf__water_box__get_water_type__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (water_box*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? (f32)((i32 (__thiscall*)(water_box*))retail::sub_61A100)(self) : 0.0f; // water_box::get_water_type

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8780E0
bool slf__water_box__contains__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (water_box*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto ent  = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    f32 result = self && ent && ent->is_an_entity_base() ? (f32)self->point_inside(ent->my_abs_po->get_position()) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878170
bool slf__water_box__surface_contains__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (water_box*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto ent  = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    f32 result = self && ent && ent->is_an_entity_base() ? (f32)self->surface_point_inside(ent->my_abs_po->get_position()) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}
