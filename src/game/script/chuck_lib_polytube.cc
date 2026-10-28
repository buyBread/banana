#include <cstring>

#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_types.hh"
#include "game/arch_base.hh"
#include "game/math/references.hh"
#include "game/polytube.hh"
#include "game/script/chuck_lib_polytube.hh"
#include "game/script/script_access.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// inlined @ sub_8645B0
// inlined @ sub_864B80
// inlined @ sub_864C10
// inlined @ sub_864CB0
// inlined @ sub_864D70
// inlined @ sub_864E30
// inlined @ sub_864ED0
// inlined @ sub_864F80
// inlined @ sub_865010
// inlined @ sub_8650E0
// inlined @ sub_865180
// inlined @ sub_865220
// inlined @ sub_865290
// inlined @ sub_865380
ai_tentacle_engine* treyarch::find_tentacle_engine(polytube* self, f32 id) {
    if (!self || !self->tentacle_info)
        return nullptr;

    return (ai_tentacle_engine*)retail::sub_4B4B90((u32*)self->tentacle_info, (i32)id); // ai_tentacle_info::find_engine
}

// inlined @ sub_864640
// inlined @ sub_864730
// inlined @ sub_864830
// inlined @ sub_864940
// inlined @ sub_864A50
f32 treyarch::link_tentacle_engine(ai_tentacle_info* info, ai_tentacle_engine* engine, f32 blend_time) { // the new engine's id
    const u32 id = (u32)retail::sub_4B4B50((u32*)info, (u32*)engine); // ai_tentacle_info::link_engine

    retail::sub_4BC320((f32*)info, 1.0f, blend_time); // ai_tentacle_info::set_code_blend

    return (f32)id;
}

// sub_842570
u32 slc_polytube_t::find_instance(const mash::string &name) const {
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

// sub_863760
bool slf__polytube__reverse_tentacle__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        retail::sub_62B420((u32*)self); // reverse_tentacle

    return true;
}

// sub_87E4F0
bool slf__polytube__reserve_control_pts__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto num  = stack.local_num(0x04);

    if (!self)
        return true;

    self->the_spline.clear_control_pts();
    self->the_spline.control_pts.reserve((i32)num);

    return true;
}

// sub_863780
bool slf__polytube__extend_to_have_widths_and_normals__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        retail::sub_65B5B0((u32*)self); // extend_to_have_widths_and_normals

    return true;
}

// sub_8637A0
bool slf__polytube__initialize_ends_closed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        retail::sub_616410((f32*)self, 0); // initialize_ends_closed

    return true;
}

// sub_8637C0
bool slf__polytube__add_control_pt__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto pt   = (const vector3d*)&stack.local_uint(0x04);

    // given in the world, kept in the polytube's own frame
    if (self)
        self->add_control_pt(self->my_abs_po->inverse_xform(*pt));

    return true;
}

// sub_8639F0
bool slf__polytube__set_control_pt__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = (i32)stack.local_num(0x04);
    auto vec   = (const vector3d*)&stack.local_uint(0x08);

    if (!self)
        return true;

    vector3d local = self->my_abs_po->inverse_xform(*vec);

    retail::sub_492A10((i32)&self->the_spline, index, (f32*)&local); // spline::set_control_pt

    return true;
}

// sub_863C30
bool slf__polytube__get_control_pt__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = (i32)stack.local_num(0x04);

    // the index isn't checked
    vector3d result = self ? self->my_abs_po->slow_xform(self->the_spline.control_pts[index]) : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_863D90
bool slf__polytube__set_additive__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto additive = stack.local_num(0x04);

    if (self)
        self->additive = additive != 0.0f;

    return true;
}

// sub_863DE0
bool slf__polytube__get_num_control_pts__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? (f32)self->get_num_control_pts() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_863E70
bool slf__polytube__build__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto subdiv = (i32)stack.local_num(0x04);
    auto type   = (i32)stack.local_num(0x08);

    if (self && type >= spline_direct && type <= spline_bezier)
        retail::sub_96F1B0((i32)&self->the_spline, subdiv, type); // spline::build

    return true;
}

// sub_863EE0
bool slf__polytube__set_material__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto file = (const char*)stack.local_uint(0x04);

    native_resource_context context(stack);

    if (self) {
        mash::string name;
        i32          type = (i32)amalga::e_resource_type::texture;

        retail::sub_72DCB0(file, (i32)&name, (u32*)&type); // resource_key::calc_resource_string_and_type_from_path
        retail::sub_615760((u32*)self, (char*)name.c_str()); // set_material
    }

    return true;
}

// sub_864000
bool slf__polytube__set_material_dynamic__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto material = (const char*)stack.local_uint(0x04);
    auto context  = (actor*)arch_base_vhandle(stack.local_uint(0x08)).resolve();

    if (!self)
        return true;

    // looked up, unstripped, in the context actor's pack, else the hero's
    actor* owner = context ? context : (actor*)references::g_world_ptr.read()->get_hero_ptr();

    amalga::push_resource_context_stack_object scope(owner->pack_slot);

    retail::sub_615760((u32*)self, (char*)material); // set_material

    return true;
}

// sub_8640D0
bool slf__polytube__set_tube_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto rad  = stack.local_num(0x04);

    if (self)
        self->set_tube_radius(rad);

    return true;
}

// sub_864120
bool slf__polytube__set_num_sides__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto n    = stack.local_num(0x04);

    if (self)
        self->set_num_sides((i32)n);

    return true;
}

// sub_864160
bool slf__polytube__set_tiles_per_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto t    = stack.local_num(0x04);

    if (self)
        self->tiles_per_meter = t;

    return true;
}

// sub_864190
bool slf__polytube__set_max_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto l    = stack.local_num(0x04);

    if (self)
        self->max_length = l;

    return true;
}

// sub_8641C0
bool slf__polytube__v_animate__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto v    = stack.local_num(0x04);

    if (!self)
        return true;

    self->v_anim = -0.0f - v;
    self->v_animate();

    return true;
}

// sub_864200
bool slf__polytube__simulate_slack__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x20;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto start_pos = (const vector3d*)&stack.local_uint(0x04);
    auto end_pos   = (const vector3d*)&stack.local_uint(0x10);
    auto length    = stack.local_num(0x1C);

    if (self)
        retail::sub_62A660((u32*)self, (f32*)start_pos, (f32*)end_pos, length); // simulate_slack

    return true;
}

// sub_864240
bool slf__polytube__create_tentacle__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        retail::sub_63A590((i32)self); // create_tentacle

    return true;
}

// sub_864260
bool slf__polytube__destroy_tentacle__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        retail::sub_64B5C0((i32)self); // destroy_tentacle

    return true;
}

// sub_864280
bool slf__polytube__tentacle_set_collide__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto collide = stack.local_num(0x04);

    if (!self || !self->tentacle_info)
        return true;

    if (collide != 0.0f)
        self->tentacle_info->flags |= ai_tentacle_info::check_collisions;
    else
        self->tentacle_info->flags &= ~ai_tentacle_info::check_collisions;

    return true;
}

// sub_8642D0
bool slf__polytube__tentacle_set_end_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto pos  = (const vector3d*)&stack.local_uint(0x04);

    if (self && self->tentacle_info)
        retail::sub_491710((f32*)self->tentacle_info, (f32*)pos); // ai_tentacle_info::set_end_position

    return true;
}

// sub_864300
bool slf__polytube__tentacle_get_end_position__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self && self->tentacle_info ?
        *(vector3d*)retail::sub_4917F0((char*)self->tentacle_info) : // ai_tentacle_info::get_end_position
        vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864380
bool slf__polytube__tentacle_get_num_positions__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->tentacle_info ? (f32)retail::sub_4915D0((u32*)self->tentacle_info) : 0.0f; // ai_tentacle_info::get_num_positions

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8643E0
bool slf__polytube__tentacle_set_position__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = (i32)stack.local_num(0x04);
    auto pos   = (const vector3d*)&stack.local_uint(0x08);

    if (self && self->tentacle_info)
        retail::sub_4A01D0((u32*)self->tentacle_info, index, (f32*)pos); // ai_tentacle_info::set_position

    return true;
}

// sub_864420
bool slf__polytube__tentacle_get_position__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = (i32)stack.local_num(0x04);

    vector3d result = self && self->tentacle_info ?
        *(vector3d*)retail::sub_4A01B0((u32*)self->tentacle_info, index) : // ai_tentacle_info::get_position
        vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8644B0
bool slf__polytube__tentacle_create_line__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto abs_position = (const vector3d*)&stack.local_uint(0x04);
    auto length       = stack.local_num(0x10);

    if (!self || !self->tentacle_info)
        return true;

    // anything shorter is a straight line
    if (0.01f > length)
        retail::sub_4B4510((i32)self->tentacle_info, (f32*)abs_position, nullptr);         // ai_tentacle_info::create_line
    else
        retail::sub_4B4730((i32)self->tentacle_info, (f32*)abs_position, length, nullptr); // ai_tentacle_info::create_sagging_line

    return true;
}

// sub_864510
bool slf__polytube__set_code_blend__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto blend = stack.local_num(0x04);
    auto time  = stack.local_num(0x08);

    if (self && self->tentacle_info)
        retail::sub_4BC320((f32*)self->tentacle_info, blend, time); // ai_tentacle_info::set_code_blend

    return true;
}

// sub_864550
bool slf__polytube__tentacle_kill_engine__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    if (self && self->tentacle_info)
        retail::sub_4B4BC0((u32*)self->tentacle_info, (i32)id); // ai_tentacle_info::kill_engine

    return true;
}

// sub_8645B0
bool slf__polytube__tentacle_engine_running__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    f32 result = find_tentacle_engine(self, id) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864640
bool slf__polytube__tentacle_add_strike_engine__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target     = (const vector3d*)&stack.local_uint(0x04);
    auto speed      = stack.local_num(0x10);
    auto blend_time = stack.local_num(0x14);

    if (!self || !self->tentacle_info)
        return true;

    void* memory = mm::allocate(0x30);

    auto engine = memory ? (ai_tentacle_engine*)retail::sub_4B9E20((u32*)memory, (i32)self->tentacle_info) : nullptr;

    ((void (__thiscall*)(ai_tentacle_engine*, const vector3d*, f32, i32))retail::sub_491CC0)(engine, target, speed, -1); // ai_tentacle_strike::setup

    f32 result = link_tentacle_engine(self->tentacle_info, engine, blend_time);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864730
bool slf__polytube__tentacle_add_swirl_strike_engine__vector3d__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x20;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target     = (const vector3d*)&stack.local_uint(0x04);
    auto speed      = stack.local_num(0x10);
    auto accel      = stack.local_num(0x14);
    auto curve_rad  = stack.local_num(0x18);
    auto blend_time = stack.local_num(0x1C);

    if (!self || !self->tentacle_info)
        return true;

    void* memory = mm::allocate(0x38);

    auto engine = memory ? (ai_tentacle_engine*)retail::sub_4B9E90((u32*)memory, (i32)self->tentacle_info) : nullptr;

    ((void (__thiscall*)(ai_tentacle_engine*, const vector3d*, f32, f32, f32))retail::sub_491CF0)(engine, target, speed, accel, curve_rad); // ai_tentacle_swirl_strike::setup

    f32 result = link_tentacle_engine(self->tentacle_info, engine, blend_time);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864830
bool slf__polytube__tentacle_add_dangle_engine__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto length     = stack.local_num(0x04);
    auto blend_time = stack.local_num(0x08);

    if (!self || !self->tentacle_info)
        return true;

    void* memory = mm::allocate(0x2C);

    auto engine = memory ? (ai_tentacle_engine*)retail::sub_4B9ED0((i32)memory, (i32)self->tentacle_info) : nullptr;

    // too short keeps the tentacle's own length
    if (length < 0.0001)
        length = -1.0f;

    ((void (__thiscall*)(ai_tentacle_engine*, f32, const vector3d*, bool, bool))retail::sub_4A07F0)(engine, length, &references::unk_010f7140.get(), true, true); // ai_tentacle_dangle::setup

    f32 result = link_tentacle_engine(self->tentacle_info, engine, blend_time);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864940
bool slf__polytube__tentacle_add_follow_engine__num__num__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slack_percent = stack.local_num(0x04);
    auto drag          = stack.local_num(0x08);
    auto gravity       = (const vector3d*)&stack.local_uint(0x0C);
    auto blend_time    = stack.local_num(0x18);

    if (!self || !self->tentacle_info)
        return true;

    void* memory = mm::allocate(0x3C);

    auto engine = memory ? (ai_tentacle_engine*)retail::sub_4B9F70((i32)memory, (i32)self->tentacle_info) : nullptr;

    ((void (__thiscall*)(ai_tentacle_engine*, f32, f32, const vector3d*, i32, bool, bool))retail::sub_4BC580)(engine, slack_percent, drag, gravity, 0, true, false); // ai_tentacle_follow::setup

    f32 result = link_tentacle_engine(self->tentacle_info, engine, blend_time);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864A50
bool slf__polytube__tentacle_add_slack_engine__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto rope_len   = stack.local_num(0x04);
    auto blend_time = stack.local_num(0x08);

    if (!self || !self->tentacle_info)
        return true;

    void* memory = mm::allocate(0x40);

    auto engine = memory ? (ai_tentacle_engine*)retail::sub_4B9FA0((i32)memory, (i32)self->tentacle_info) : nullptr;

    // hanging under the world's gravity
    vector3d gravity(0.0f, -0.0f - references::gravity.read(), 0.0f);

    ((void (__thiscall*)(ai_tentacle_engine*, f32, f32, const vector3d*, i32, bool, bool))retail::sub_4BC690)(engine, rope_len, 1.0f, &gravity, 0, true, false); // ai_tentacle_slack::setup

    f32 result = link_tentacle_engine(self->tentacle_info, engine, blend_time);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864B80
bool slf__polytube__tentacle_set_engine_attach_ent__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);
    auto ent  = stack.local_uint(0x08);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    if (!engine)
        return true;

    if (engine->get_type() == tentacle_engine_follow || engine->get_type() == tentacle_engine_slack)
        retail::sub_491EF0((i32)engine, (i32)ent); // ai_tentacle_follow::set_attach_ent

    return true;
}

// sub_864C10
bool slf__polytube__tentacle_set_engine_attach_ent__num__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);
    auto ent       = stack.local_uint(0x08);
    auto local_end = (const vector3d*)&stack.local_uint(0x0C);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    if (!engine)
        return true;

    if (engine->get_type() == tentacle_engine_follow || engine->get_type() == tentacle_engine_slack)
        retail::sub_492060((i32)engine, (i32)ent, (f32*)local_end); // ai_tentacle_follow::set_attach_ent

    return true;
}

// sub_864CB0
bool slf__polytube__tentacle_set_engine_target__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);
    auto target = (const vector3d*)&stack.local_uint(0x08);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    if (!engine)
        return true;

    if (engine->get_type() == tentacle_engine_strike)
        ((ai_tentacle_strike*)engine)->target = *target;
    else if (engine->get_type() == tentacle_engine_swirl_strike)
        ((ai_tentacle_strike*)engine)->target = *target;

    return true;
}

// sub_864D70
bool slf__polytube__tentacle_get_engine_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    f32 result = 0.0f;

    if (engine && (engine->get_type() == tentacle_engine_dangle || engine->get_type() == tentacle_engine_slack))
        result = engine->get_length();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864E30
bool slf__polytube__tentacle_set_engine_length__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);
    auto length = stack.local_num(0x08);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    if (engine && (engine->get_type() == tentacle_engine_dangle || engine->get_type() == tentacle_engine_slack))
        engine->set_length(length);

    return true;
}

// sub_864ED0
bool slf__polytube__tentacle_get_engine_slack_percent__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    f32 result = engine && engine->get_type() == tentacle_engine_follow ? engine->get_slack_percent() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_864F80
bool slf__polytube__tentacle_set_engine_slack_percent__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);
    auto percent = stack.local_num(0x08);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    // a slack engine overrides the slot too, but only follows take it
    if (engine && engine->get_type() == tentacle_engine_follow)
        engine->set_slack_percent(percent);

    return true;
}

// sub_865010
bool slf__polytube__tentacle_get_engine_drag__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    f32 result = 0.0f;

    if (engine) {
        if (engine->get_type() == tentacle_engine_follow)
            result = ((ai_tentacle_follow*)engine)->follow_drag;
        else if (engine->get_type() == tentacle_engine_dangle)
            result = (f32)retail::sub_491E60((u32*)engine); // ai_tentacle_dangle_old::get_drag
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8650E0
bool slf__polytube__tentacle_set_engine_drag__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);
    auto drag = stack.local_num(0x08);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    if (!engine)
        return true;

    if (engine->get_type() == tentacle_engine_follow)
        ((void (__thiscall*)(ai_tentacle_engine*, f32))retail::sub_492080)(engine, drag); // ai_tentacle_follow::set_drag
    else if (engine->get_type() == tentacle_engine_dangle)
        ((void (__thiscall*)(ai_tentacle_engine*, f32))retail::sub_491E70)(engine, drag); // ai_tentacle_dangle_old::set_drag

    return true;
}

// sub_865180
bool slf__polytube__tentacle_get_engine_elasticity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    // any engine is taken for a dangle
    f32 result = engine ? (f32)retail::sub_491E90((u32*)engine) : 0.0f; // ai_tentacle_dangle_old::get_elasticity

    stack.push(&result, sizeof(result));

    return true;
}

// sub_865220
bool slf__polytube__tentacle_set_engine_elasticity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);
    auto elasticity = stack.local_num(0x08);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    // any engine is taken for a dangle
    if (engine)
        ((void (__thiscall*)(ai_tentacle_engine*, f32))retail::sub_491DC0)(engine, elasticity); // ai_tentacle_dangle_old::set_elasticity

    return true;
}

// sub_865290
bool slf__polytube__tentacle_get_engine_gravity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    vector3d result = references::unk_010f7140.get();

    if (engine && (engine->get_type() == tentacle_engine_follow || engine->get_type() == tentacle_engine_dangle))
        retail::sub_491E20((u32*)engine, (i32)&result); // ai_tentacle_dangle_old::get_gravity

    stack.push(&result, sizeof(result));

    return true;
}

// sub_865380
bool slf__polytube__tentacle_set_engine_gravity__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);
    auto gravity = (const vector3d*)&stack.local_uint(0x08);

    ai_tentacle_engine* engine = find_tentacle_engine(self, id);

    if (engine && (engine->get_type() == tentacle_engine_follow || engine->get_type() == tentacle_engine_dangle))
        retail::sub_491DA0((u32*)engine, (f32*)gravity); // ai_tentacle_dangle_old::set_gravity

    return true;
}

// sub_865420
bool slf__polytube__get_spline_pt__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto percent = &stack.local_num(0x04);
    auto type    = (i32)stack.local_num(0x08);

    if (!self) {
        vector3d result = references::unk_010f7140.get();

        stack.push(&result, sizeof(result));

        return true;
    }

    // clamped in place
    if (0.0f > *percent)
        *percent = 0.0f;
    else if (*percent > 1.0f)
        *percent = 1.0f;

    if (type < spline_direct || type > spline_bezier)
        type = spline_default;

    vector3d local;
    retail::sub_97A3B0(&self->the_spline, (f32*)&local, *percent, type); // spline::get_spline_pt

    vector3d result = self->my_abs_po->slow_xform(local);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_865620
bool slf__polytube__tentacle_web_set_up__entity__uint__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (polytube*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto owner        = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();
    auto use_hero_web = stack.local_uint(0x08);
    auto radius       = stack.local_num(0x0C);

    if (!self)
        return true;

    // the owner isn't checked; the hero dresses the tube as his own web unless he's in black mode and wasn't asked to
    if (owner->has_ifc(entity_ifc_tentacle)) {
        bool own_tentacles = !retail::sub_601C50((u32*)owner); // entity_base::is_hero

        if (!own_tentacles) {
            player_interface* player = owner->player_ifc();

            own_tentacles = ((bool (__thiscall*)(player_interface*))(*(void***)player)[305])(player) && !use_hero_web; // slot 305, black mode
        }

        if (own_tentacles) {
            auto tentacle = (u32*)owner->my_ifc_storage->get_ifc(entity_ifc_tentacle);

            retail::sub_65D990(tentacle, (i32)self, radius); // tentacle_interface::tentacle_web_set_up

            return true;
        }
    }

    if (retail::sub_601C50((u32*)owner)) {
        player_interface* player = owner->player_ifc();

        ((void (__thiscall*)(player_interface*, polytube*, f32))(*(void***)player)[400])(player, self, radius); // slot 400, dress_polytube_as_web
    }

    return true;
}
