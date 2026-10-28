#include <cstring>

#include "retail.hh"
#include "game/amalga/resource_types.hh"
#include "game/arch_base.hh"
#include "game/math/references.hh"
#include "game/script/chuck_lib_web_wall.hh"
#include "game/script/script_access.hh"
#include "game/web_wall.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_842570
u32 slc_web_wall_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    // slot 77
    if (entity)
        ((void (__thiscall*)(arch_base*))entity->vtable[77])(entity);

    return entity->my_handle.value;
}

bool slf__create_web_wall__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8425F0)(this, stack);
}

// sub_8789E0
bool slf__web_wall__clear__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (web_wall*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        retail::sub_61DED0((char*)self); // web_wall::clear

    return true;
}

// sub_878A00
bool slf__web_wall__fire__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (web_wall*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = stack.local_uint(0x04);
    auto scale  = stack.local_num(0x08);
    auto unk    = stack.local_num(0x0C);

    f32 result = self && ((bool (__thiscall*)(web_wall*, u32, f32, f32))retail::sub_672F30)(self, target, scale, unk) ? 1.0f : 0.0f; // web_wall::fire_at_entity

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878AA0
bool slf__web_wall__fire__vector3d__vector3d__vector3d__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x3C;

    auto self = (web_wall*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto corners = (const vector3d*)&stack.local_uint(0x04);
    auto scale   = stack.local_num(0x34);
    auto unk     = stack.local_num(0x38);

    using fire_t = bool (__thiscall*)(web_wall*, vector3d, vector3d, vector3d, vector3d, f32, f32);

    f32 result = self && ((fire_t)retail::sub_671DB0)(self, corners[0], corners[1], corners[2], corners[3], scale, unk) ? 1.0f : 0.0f; // web_wall::fire

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878B90
bool slf__web_wall__set_material__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (web_wall*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto material = (const char*)stack.local_uint(0x04);

    native_resource_context context(stack);

    if (self) {
        mash::string name;
        i32          type = (i32)amalga::e_resource_type::texture;

        retail::sub_72DCB0(material, (i32)&name, (u32*)&type); // resource_key::calc_resource_string_and_type_from_path

        std::strncpy(self->material, name.c_str(), sizeof(self->material));
    }

    return true;
}

// sub_878CB0
bool slf__web_wall__is_active__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (web_wall*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->wall_active ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878D30
bool slf__web_wall__is_building__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (web_wall*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->build_progress < web_wall::built ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_878DB0
bool slf__web_wall__get_build_point__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (web_wall*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->build_point : references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}
