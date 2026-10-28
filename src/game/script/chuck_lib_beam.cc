#include <cstring>

#include "retail.hh"
#include "game/arch_base.hh"
#include "game/beam.hh"
#include "game/script/chuck_lib_beam.hh"
#include "game/script/script_access.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_83E080
u32 slc_beam_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    return entity->my_handle.value;
}

// sub_8569D0
bool slf__beam__hit_world__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(beam::hit_world) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_856A50
bool slf__beam__hit_hero__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(beam::hit_hero) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_856AD0
bool slf__beam__get_impact_point__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->impact_point : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_856B40
bool slf__beam__get_impact_normal__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->impact_normal : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_856BB0
bool slf__beam__set_thickness__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self)
        self->set_thickness(value);

    return true;
}

// sub_856BE0
bool slf__beam__set_max_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self)
        self->set_max_length(value);

    return true;
}

// sub_856C10
bool slf__beam__set_color__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self)
        return true;

    // each channel truncated, then kept within a byte
    color32 value;

    self->set_beam_color(value.set_clamped((i32)stack.local_num(0x04),
                                           (i32)stack.local_num(0x08),
                                           (i32)stack.local_num(0x0C),
                                           (i32)stack.local_num(0x10)));

    return true;
}

// sub_856C60
bool slf__beam__set_collide_hero__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on != 0.0f)
        self->clear_flag(beam::no_collide_hero);
    else
        self->set_flag(beam::no_collide_hero);

    return true;
}

// sub_856CB0
bool slf__beam__set_clip_to_hero__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on != 0.0f)
        self->clear_flag(beam::no_clip_to_hero);
    else
        self->set_flag(beam::no_clip_to_hero);

    return true;
}

// sub_856D00
bool slf__beam__set_collide_world__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on != 0.0f)
        self->clear_flag(beam::no_collide_world);
    else
        self->set_flag(beam::no_collide_world);

    return true;
}

// sub_856D50
bool slf__beam__set_clip_to_world__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on != 0.0f)
        self->clear_flag(beam::no_clip_to_world);
    else
        self->set_flag(beam::no_clip_to_world);

    return true;
}

// sub_856DA0
bool slf__beam__set_world_entity_collision__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on != 0.0f)
        self->set_flag(beam::world_entity_collision);
    else
        self->clear_flag(beam::world_entity_collision);

    return true;
}

// sub_856DF0
bool slf__beam__set_collide_beamable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on != 0.0f)
        self->clear_flag(beam::no_clip_to_beamable);
    else
        self->set_flag(beam::no_clip_to_beamable);

    return true;
}

// sub_856E40
bool slf__beam__set_no_collision__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->set_flag(beam::no_clipping);

    return true;
}

// sub_856E70
bool slf__beam__set_additive__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self)
        self->additive = value != 0.0f;

    return true;
}

// sub_856EC0
bool slf__beam__set_point_to_point__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto from = (const vector3d*)&stack.local_uint(0x04);
    auto to   = (const vector3d*)&stack.local_uint(0x10);

    if (self)
        self->set_point_to_point(*from, *to);

    return true;
}

// sub_856EF0
bool slf__beam__set_uv_anim__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->set_uv_anim(vector2d(stack.local_num(0x04), stack.local_num(0x08)));

    return true;
}

// sub_856F80
bool slf__beam__set_tiles_per_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self)
        self->tiles_per_meter = value;

    return true;
}

// sub_856FB0
bool slf__beam__set_material__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (beam*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto material = (const char*)stack.local_uint(0x04);

    native_resource_context context(stack);

    if (self)
        self->set_material(mash::string(material));

    return true;
}
