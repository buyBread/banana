#include "retail.hh"
#include "game/als/animation_logic_system.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

string_hash actor::my_type_key { 0x07138E99 }; // "actor"

// sub_6651E0
actor::~actor() {
    finalize(mash::ALLOCATED);
}

void actor::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(actor*, mash::allocation_scope))retail::sub_65AF90)(this, scope);
}

// slot 1
void actor::construct_mashed_class() {
    ((void (__thiscall*)(actor*))retail::sub_654100)(this);
}

// slot 2
void actor::destruct_mashed_class() {
    ((void (__thiscall*)(actor*))retail::sub_667470)(this);
}

// slot 3
void actor::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(actor*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_64D2D0)(this,
                                                                                                        mash_info,
                                                                                                        containing_class_ptr,
                                                                                                        buffer);
}

// slot 4
mash::virtual_types_key actor::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const actor*))retail::sub_6653A0)(this);
}

// slot 6
bool actor::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const actor*, mash::virtual_types_key))retail::sub_71B670)(this, parent_class);
}

// slot 8
i32 actor::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const actor*))retail::sub_7187F0)(this);
}

// slot 12
bool actor::is_an_actor() const {
    return ((bool (__thiscall*)(const actor*))retail::sub_667F80)(this);
}

// slot 53
vector3d actor::get_last_position() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const actor*, vector3d*))retail::sub_602E60)(this, &result);

    return result;
}

// slot 68
collision_geometry* actor::get_colgeom() const {
    return ((collision_geometry* (__thiscall*)(const actor*))retail::sub_68B9C0)(this);
}

// slot 69
void actor::set_collisions_active(bool active, bool recursive) {
    ((void (__thiscall*)(actor*, bool, bool))retail::sub_65B1D0)(this, active, recursive);
}

// slot 70
bool actor::are_character_collisions_active() const {
    return ((bool (__thiscall*)(const actor*))retail::sub_6027F0)(this);
}

// slot 76
bool actor::is_alive() const {
    return ((bool (__thiscall*)(const actor*))retail::sub_614E90)(this);
}

// slot 77
e_entity_flavor actor::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const actor*))retail::sub_A944D0)(this);
}

// slot 78
bool actor::is_a_placeholder_vehicle() const {
    return ((bool (__thiscall*)(const actor*))retail::sub_6025E0)(this);
}

// slot 86
void actor::post_unmash_fixup(amalga::resource_pack_slot* pack_slot) {
    ((void (__thiscall*)(actor*, amalga::resource_pack_slot*))retail::sub_629810)(this, pack_slot);
}

// slot 87
u32 actor::get_entity_size() const {
    return ((u32 (__thiscall*)(const actor*))retail::sub_7187F0)(this);
}

// slot 108
void actor::unk_108(f32 unk) {
    ((void (__thiscall*)(actor*, f32))retail::sub_639700)(this, unk);
}

// slot 109
void actor::render(f32 camera_distance, f32 entity_translucency_pct) {
    ((void (__thiscall*)(actor*, f32, f32))retail::sub_627720)(this, camera_distance, entity_translucency_pct);
}

// slot 110
ngl::mesh* actor::get_mesh() const {
    return ((ngl::mesh* (__thiscall*)(const actor*))retail::sub_665240)(this);
}

// slot 111
ngl::mesh* actor::unk_111() const {
    return ((ngl::mesh* (__thiscall*)(const actor*))retail::sub_665240)(this);
}

// slot 112
bool actor::has_mesh() const {
    return ((bool (__thiscall*)(const actor*))retail::sub_665250)(this);
}

// slot 113
f32 actor::get_visual_radius() const {
    return ((f32 (__thiscall*)(const actor*))retail::sub_602860)(this);
}

// slot 114
vector3d actor::get_visual_center() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const actor*, vector3d*))retail::sub_602990)(this, &result);

    return result;
}

// sub_602EB0
void actor::suspend(bool update_region) {
    if (flags & entity_flag_suspended)
        return;

    flags |= entity_flag_suspended;

    if (my_base_ai_data && my_base_ai_data->core)
        my_base_ai_data->core->push_disable(0);

    flags &= ~entity_flag_moving;
}

// sub_602EF0
void actor::unsuspend(bool update_region) {
    if (!(flags & entity_flag_suspended))
        return;

    flags &= ~entity_flag_suspended;

    if (my_base_ai_data && my_base_ai_data->core)
        my_base_ai_data->core->pop_disable(0);

    flags |= entity_flag_moving;
}

// slot 118
void actor::set_render_color(color32 color) {
    ((void (__thiscall*)(actor*, color32))retail::sub_620F50)(this, color);
}

// slot 119
color32 actor::get_render_color() const {
    color32 result;
    ((color32* (__thiscall*)(const actor*, color32*))retail::sub_6653B0)(this, &result);

    return result;
}

// slot 120
void actor::set_render_alpha_mod(f32 alpha) {
    ((void (__thiscall*)(actor*, f32))retail::sub_620FC0)(this, alpha);
}

// slot 121
f32 actor::get_render_alpha_mod() const {
    return ((f32 (__thiscall*)(const actor*))retail::sub_665270)(this);
}

// slot 122
void actor::set_render_scale(const vector3d &scale) {
    ((void (__thiscall*)(actor*, const vector3d&))retail::sub_621040)(this, scale);
}

// slot 123
vector3d actor::get_render_scale() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const actor*, vector3d*))retail::sub_6652B0)(this, &result);

    return result;
}

// slot 124
void actor::set_render_zbias(f32 zbias) {
    ((void (__thiscall*)(actor*, f32))retail::sub_6653F0)(this, zbias);
}

// slot 125
f32 actor::get_render_zbias() const {
    return ((f32 (__thiscall*)(const actor*))retail::sub_665300)(this);
}

// slot 134
bool actor::hates_spiderman() {
    return ((bool (__thiscall*)(actor*))retail::sub_602CF0)(this);
}

// slot 136
bool actor::is_a_rocket() const {
    return ((bool (__thiscall*)(const actor*))retail::sub_71F870)(this);
}

// slot 137
void actor::allocate_anim_controller() {
    ((void (__thiscall*)(actor*))retail::sub_6024B0)(this);
}

// slot 138
f32 actor::get_floor_offset() {
    return ((f32 (__thiscall*)(actor*))retail::sub_602520)(this);
}

// slot 139
void actor::set_floor_offset(f32 offset) {
    ((void (__thiscall*)(actor*, f32))retail::sub_689610)(this, offset);
}

// slot 140
void* actor::bind_to_scene_anim(nal::scene_anim_instance* instance, i32 index) {
    return ((void* (__thiscall*)(actor*, nal::scene_anim_instance*, i32))retail::sub_602540)(this, instance, index);
}

// slot 141
void actor::unbind_from_scene_anim() {
    ((void (__thiscall*)(actor*))retail::sub_602570)(this);
}

// slot 142
bool actor::unk_142() {
    return ((bool (__thiscall*)(actor*))retail::sub_613D10)(this);
}

// slot 143
void actor::set_colgeom_flag_recursive(u32 flag, bool set) {
    ((void (__thiscall*)(actor*, u32, bool))retail::sub_602660)(this, flag, set);
}

// slot 144
void actor::update_colgeom(po* replacement_po) {
    ((void (__thiscall*)(actor*, po*))retail::sub_602720)(this, replacement_po);
}

// slot 145
void actor::invalidate_colgeom() {
    ((void (__thiscall*)(actor*))retail::sub_602760)(this);
}

// slot 146
f32 actor::get_inter_capsule_radius_scale() {
    return ((f32 (__thiscall*)(actor*))retail::sub_720040)(this);
}

// slot 147
bool actor::test_combat_target(const vector3d  &p0,
                               const vector3d  &p1,
                                     vector3d*  impact_pos,
                                     vector3d*  impact_normal,
                                     f32        default_radius,
                                     bool       rear_cull) const {

    using test_combat_target_method = bool (__thiscall*)(const actor*, const vector3d&, const vector3d&, vector3d*, vector3d*, f32, bool);

    return ((test_combat_target_method)retail::sub_613D80)(this,
                                                           p0,
                                                           p1,
                                                           impact_pos,
                                                           impact_normal,
                                                           default_radius,
                                                           rear_cull);
}

// slot 148
f32 actor::get_colgeom_radius() const {
    return ((f32 (__thiscall*)(const actor*))retail::sub_602C90)(this);
}

// slot 149
vector3d actor::get_colgeom_center() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const actor*, vector3d*))retail::sub_602B10)(this, &result);

    return result;
}

// slot 150
void actor::radius_changed(bool force) {
    ((void (__thiscall*)(actor*, bool))retail::sub_602CB0)(this, force);
}

// slot 151
ngl::morph_set* actor::get_morph(const tl_fixed_string &morph_name, bool warn_if_missing) {
    return ((ngl::morph_set* (__thiscall*)(actor*, const tl_fixed_string&, bool))retail::sub_614C20)(this, morph_name, warn_if_missing);
}

// slot 152
bool actor::is_frame_delta_valid() const {
    return ((bool (__thiscall*)(const actor*))retail::sub_665340)(this);
}

// slot 153
bool actor::is_last_frame_delta_valid() const {
    return ((bool (__thiscall*)(const actor*))retail::sub_665360)(this);
}

// slot 154
const po &actor::get_frame_delta() const {
    return ((const po& (__thiscall*)(const actor*))retail::sub_665380)(this);
}

// slot 155
void actor::set_frame_delta(const po &bob, f32 t) {
    ((void (__thiscall*)(actor*, const po&, f32))retail::sub_6293E0)(this, bob, t);
}

// slot 156
void actor::set_frame_delta_trans(const vector3d &delta, f32 delta_t) {
    ((void (__thiscall*)(actor*, const vector3d&, f32))retail::sub_629420)(this, delta, delta_t);
}

// slot 157
void actor::invalidate_frame_delta() {
    ((void (__thiscall*)(actor*))retail::sub_602D30)(this);
}

// sub_602830
// inlined @ sub_611A90
ai_core* actor::get_ai_core() const {
    return my_base_ai_data ? my_base_ai_data->core : nullptr;
}

// sub_602F30
als::animation_logic_system_interface* actor::get_als() const {
    if (adv_ptrs && adv_ptrs->my_als_data)
        return adv_ptrs->my_als_data->the_als;

    return nullptr;
}

void actor::set_frame_delta_no_update(const po &bob, f32 t) {
    retail::sub_629340((u32*)this, (f32*)&bob, t);
}

void actor::save_last_collision_free_state() {
    retail::sub_6299F0((u32*)this);
}
