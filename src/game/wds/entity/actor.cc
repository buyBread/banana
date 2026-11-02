#include <cmath>
#include <new>

#include "retail.hh"
#include "game/als/animation_logic_system.hh"
#include "game/amalga/resource_types.hh"
#include "game/colgeom.hh"
#include "game/math/references.hh"
#include "game/resource_key.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_team.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/capsule_alter_sys.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/conglomerate_clone.hh"
#include "treyarch/jq/jq.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/ngl/mesh/mesh.hh"

using namespace treyarch;

string_hash actor::my_type_key { 0x07138E99 }; // "actor"

// sub_4C4B10
void base_ai_data::construct_mashed_class() {
    core_ai_key.construct_mashed_class();
    overrides  .construct_mashed_class();
}

// sub_6396A0
void advanced_entity_ptrs::construct_mashed_class() {
    script_obj_name.construct_mashed_class();

    if (script_exe_name)
        script_exe_name->construct_mashed_class();

    if (my_als_data)
        my_als_data->construct_mashed_class();

    initialize(mash::FROM_MASH);
}

// sub_602330
// inlined @ sub_6396A0
void advanced_entity_ptrs::initialize(mash::allocation_scope scope) {
    ignore_col_ents  = nullptr;
    m_render_info    = nullptr;
    mi               = nullptr;
    my_script        = nullptr;
    script_pack_slot = nullptr;
    unk_01c          = 0;

    if (scope != mash::ALLOCATED)
        return;

    script_exe_name = new (mm::allocate(sizeof(resource_key))) resource_key();

    string_hash no_name;
    no_name.initialize(mash::ALLOCATED);

    script_exe_name->set(no_name, (u32)amalga::e_resource_type::script);

    my_als_data = nullptr;
    unk_024     = 0.0f;
    unk_028     = -1;
}

// inlined @ sub_614190
void movement_info::add_frame_delta_velocity(vector3d* target) const { // the frame delta's translation over the frame time
    const f32 inverse_time = (f32)(1.0 / (f64)frame_time);

    const vector4d &delta = frame_delta.get_matrix().w;

    const f32 x = (f32)((f64)delta.x * inverse_time);
    const f32 y = (f32)((f64)delta.y * inverse_time);
    const f32 z = (f32)((f64)delta.z * inverse_time);

    target->x = (f32)((f64)target->x + x);
    target->y = (f32)((f64)target->y + y);
    target->z = (f32)((f64)target->z + z);
}

// sub_6651E0
actor::~actor() {
    finalize(mash::ALLOCATED);
}

void actor::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(actor*, mash::allocation_scope))retail::sub_65AF90)(this, scope);
}

// sub_613830
void actor::initialize(mash::allocation_scope scope) {
    if (scope == mash::ALLOCATED) {
        colgeom         = nullptr;
        mesh            = nullptr;
        adv_ptrs        = nullptr;
        m_skeleton      = nullptr;
        skeleton_map    = nullptr;
        my_base_ai_data = nullptr;
        capsule_alter   = nullptr;
        m_skeleton_name = nullptr;
        pack_slot       = nullptr;
        unk_0dc         = 0;

        const u32 seed = references::unk_00e6b258.read() * 1103515245 + 12345;

        references::unk_00e6b258.write(seed);

        u32 tempered = seed ^ (seed >> 11);

        tempered ^= (tempered & 0xFF3A58AD) << 7;
        tempered ^= (tempered & 0xFFFFDF8C) << 15;

        unk_0e0 = tempered * 1103515245 + 12345;
        unk_0e4 = 0;
    } else {
        if (colgeom)
            colgeom->owner = this;

        if (has_physical_ifc())
            flags |= entity_flag_moving;

        if (get_colgeom())
            set_collisions_active(true, true);

        if (capsule_alter)
            retail::sub_7E9420((f32*)capsule_alter, (i32)this); // capsule_alter_sys::set_my_actor

        ngl::mesh* my_mesh = unk_111();

        if (my_mesh && my_mesh->skeleton && my_mesh->skeleton->unk_008 > 0)
            ext_flags |= entity_ext_flag_recordable;

        start_recording();
    }

    if ((ext_flags & entity_ext_flag_is_auto_activate_prop) && !is_a_conglomerate()) {
        if (!get_colgeom() ||
            (get_colgeom()->get_type() != collision_geometry_type_mesh &&
             get_colgeom()->get_type() != collision_geometry_type_convex_hull))

            ext_flags &= ~entity_ext_flag_is_auto_activate_prop;
    }

    ext_flags &= ~(entity_ext_flag_added_to_list_of_moved_entities | entity_ext_flag_alloc_adv_ptrs);

    anim_ctrl                 = nullptr;
    last_collision_free_state = nullptr;

    if (get_colgeom()) {
        if ((ext_flags & entity_ext_flag_is_auto_activate_prop) ||
            ((flags & entity_flag_member) && (get_conglom_owner()->ext_flags & entity_ext_flag_is_auto_activate_prop)))

            flags |= entity_flag_walkable;
    }

    rb_cc = nullptr;
}

// sub_654100
void actor::construct_mashed_class() {
    entity::construct_mashed_class();

    if (m_skeleton_name)
        m_skeleton_name->construct_mashed_class();

    if (adv_ptrs)
        adv_ptrs->construct_mashed_class();

    if (colgeom)
        mash::mash_info_struct::construct_class(colgeom);

    if (my_base_ai_data)
        my_base_ai_data->construct_mashed_class();

    if (capsule_alter)
        capsule_alter->construct_mashed_class();

    initialize(mash::FROM_MASH);
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

// sub_602E60
vector3d actor::get_last_position() const {
    if (adv_ptrs && adv_ptrs->mi)
        return adv_ptrs->mi->last_position;

    return my_abs_po->get_position();
}

// sub_68B9C0
collision_geometry* actor::get_colgeom() const {
    return colgeom;
}

// slot 69
void actor::set_collisions_active(bool active, bool recursive) {
    ((void (__thiscall*)(actor*, bool, bool))retail::sub_65B1D0)(this, active, recursive);
}

// sub_6027F0
bool actor::are_character_collisions_active() const {
    return (flags & entity_flag_physics_collide_characters) && get_colgeom() && (get_colgeom()->flags & fi_collide_characters);
}

// sub_614E90
bool actor::is_alive() const {
    ai_core* core = get_ai_core();

    if (core)
        return core->alive;

    return !has_damage_ifc() || damage_ifc()->hit_points.val > 0;
}

// slot 77
e_entity_flavor actor::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const actor*))retail::sub_A944D0)(this);
}

// sub_6025E0
bool actor::is_a_placeholder_vehicle() const {
    ngl::mesh* my_mesh = get_mesh();

    if (!my_mesh)
        return false;

    mash::string name(my_mesh->name.get_text());

    return name.find("vehicle_placeholder", 0) != mash::string::npos;
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

// sub_665240
ngl::mesh* actor::get_mesh() const {
    return mesh;
}

// slot 111
ngl::mesh* actor::unk_111() const { // slot 110's body
    return mesh;
}

// sub_665250
bool actor::has_mesh() const {
    return get_mesh() != nullptr;
}

// sub_602860
f32 actor::get_visual_radius() const {
    if (!get_mesh()) {
        if (is_a_conglomerate() || (flags & entity_flag_member)) {
            conglomerate* owner = get_conglom_owner();

            if (owner)
                return owner->get_visual_radius();
        }

        return 0.0f;
    }

    const vector3d scale = get_render_scale();

    ngl::mesh* my_mesh = get_mesh();

    const f32 scale_x = std::fabs(scale.x);
    const f32 scale_y = std::fabs(scale.y);
    const f32 scale_z = std::fabs(scale.z);

    f32 largest = scale_z;

    if (scale_y > scale_z)
        largest = scale_y;

    if (scale_x > largest)
        return my_mesh->sphere.get_w() * scale_x;

    return my_mesh->sphere.get_w() * largest;
}

// sub_602990
vector3d actor::get_visual_center() const {
    if (!get_mesh())
        return my_abs_po->get_position();

    if (ext_flags & entity_ext_flag_bounding_sphere_dirty) {
        const math::vector4 &sphere = get_mesh()->sphere;

        const f32 x = sphere.get_x();
        const f32 y = sphere.get_y();
        const f32 z = sphere.get_z();

        const matrix4x4 &m = my_abs_po->get_matrix();

        cached_visual_bounding_sphere_center = vector3d((f32)((f64)m.y.x * y + (f64)m.x.x * x + (f64)m.z.x * z + m.w.x),
                                                        (f32)((f64)m.y.y * y + (f64)m.x.y * x + (f64)m.z.y * z + m.w.y),
                                                        (f32)((f64)m.y.z * y + (f64)m.x.z * x + (f64)m.z.z * z + m.w.z));

        ext_flags &= ~entity_ext_flag_bounding_sphere_dirty;
    }

    return cached_visual_bounding_sphere_center;
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

// sub_620F50
void actor::set_render_color(color32 color) {
    create_render_info()->render_color = color;
}

// sub_6653B0
color32 actor::get_render_color() const {
    return adv_ptrs && adv_ptrs->m_render_info ? adv_ptrs->m_render_info->render_color : color32(255, 255, 255, 255);
}

// sub_620FC0
void actor::set_render_alpha_mod(f32 alpha) {
    create_render_info()->render_alpha_mod = alpha;
}

// sub_665270
f32 actor::get_render_alpha_mod() const {
    return adv_ptrs && adv_ptrs->m_render_info ? adv_ptrs->m_render_info->render_alpha_mod : 1.0f;
}

// sub_621040
void actor::set_render_scale(const vector3d &scale) {
    create_render_info()->render_scale = scale;
}

// sub_6652B0
vector3d actor::get_render_scale() const {
    return adv_ptrs && adv_ptrs->m_render_info ? adv_ptrs->m_render_info->render_scale : vector3d(1.0f, 1.0f, 1.0f);
}

// sub_6653F0
void actor::set_render_zbias(f32 zbias) {
    create_render_info()->render_zbias = zbias;
}

// sub_665300
f32 actor::get_render_zbias() const {
    return adv_ptrs && adv_ptrs->m_render_info ? adv_ptrs->m_render_info->render_zbias : 1.0f;
}

// sub_602CF0
bool actor::hates_spiderman() {
    auto team = (ai_team::e_team)retail::sub_42B120((i32)get_ai_core()); // the core's team, num_teams without a core

    return team != ai_team::num_teams && ai_team::is_enemy(team, ai_team::team_spiderman);
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

// sub_613D10
bool actor::is_inside_car() {
    ai_core* core = get_ai_core();

    if (!core)
        return false;

    ai_basic_inode* inode = get_ai_basic_inode_if_slim_actor(this);

    if (inode)
        return inode->is_inside_car();

    auto car = (ai_car_inode*)core->get_info_node(info_node_type_ai_car);

    return car && car->is_in_car;
}

// sub_602660
void actor::set_colgeom_flag_recursive(u32 flag, bool set) {
    if (!get_colgeom())
        return;

    if (get_colgeom()->get_type() != collision_geometry_type_mesh &&
        get_colgeom()->get_type() != collision_geometry_type_convex_hull)

        return;

    collision_geometry* geometry = get_colgeom();

    if (set)
        geometry->flags |= flag;
    else
        geometry->flags &= ~flag;
}

// sub_602720
void actor::update_colgeom(po* replacement_po) {
    collision_geometry* geometry = get_colgeom();

    if (geometry)
        geometry->xform(replacement_po ? *replacement_po : *my_abs_po);
}

// sub_602760
void actor::invalidate_colgeom() {
    if (get_colgeom())
        get_colgeom()->valid = false;
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

// sub_602C90
f32 actor::get_colgeom_radius() const {
    if (colgeom)
        return colgeom->get_bounding_sphere_radius();

    return 0.0f;
}

// sub_602B10
vector3d actor::get_colgeom_center() const { // also writes the abs po's fourth column
    if (!colgeom)
        return my_abs_po->get_position();

    const vector3d center = colgeom->get_local_space_bounding_sphere_center();

    matrix4x4 &m = my_abs_po->get_matrix();

    m.x.w = 0.0f;
    m.y.w = 0.0f;
    m.z.w = 0.0f;
    m.w.w = 1.0f;

    return vector3d((f32)((f64)m.y.x * center.y + (f64)m.x.x * center.x + (f64)m.z.x * center.z + m.w.x),
                    (f32)((f64)m.y.y * center.y + (f64)m.x.y * center.x + (f64)m.z.y * center.z + m.w.y),
                    (f32)((f64)m.y.z * center.y + (f64)m.x.z * center.x + (f64)m.z.z * center.z + m.w.z));
}

// sub_602CB0
void actor::radius_changed(bool force) {
    ext_flags |= entity_ext_flag_bounding_sphere_dirty;
}

// slot 151
ngl::morph_set* actor::get_morph(const tl_fixed_string &morph_name, bool warn_if_missing) {
    return ((ngl::morph_set* (__thiscall*)(actor*, const tl_fixed_string&, bool))retail::sub_614C20)(this, morph_name, warn_if_missing);
}

// sub_665340
bool actor::is_frame_delta_valid() const {
    return adv_ptrs && adv_ptrs->mi && adv_ptrs->mi->frame_delta_valid;
}

// sub_665360
bool actor::is_last_frame_delta_valid() const {
    return adv_ptrs && adv_ptrs->mi && adv_ptrs->mi->last_frame_delta_valid;
}

// sub_665380
const po &actor::get_frame_delta() const {
    if (adv_ptrs && adv_ptrs->mi)
        return adv_ptrs->mi->frame_delta;

    return references::unk_010038a0.get();
}

// slot 155
void actor::set_frame_delta(const po &bob, f32 t) {
    ((void (__thiscall*)(actor*, const po&, f32))retail::sub_6293E0)(this, bob, t);
}

// slot 156
void actor::set_frame_delta_trans(const vector3d &delta, f32 delta_t) {
    ((void (__thiscall*)(actor*, const vector3d&, f32))retail::sub_629420)(this, delta, delta_t);
}

// sub_602D30
void actor::invalidate_frame_delta() {
    if (!adv_ptrs || !adv_ptrs->mi)
        return;

    movement_info* mi = adv_ptrs->mi;

    mi->last_frame_delta_valid = mi->frame_delta_valid;
    mi->frame_delta_valid      = false;
    mi->frame_delta            = references::unk_010038a0.get();

    const vector3d &position = my_abs_po->get_position();

    const f32 x = (f32)((f64)mi->last_position.x - position.x);
    const f32 y = (f32)((f64)mi->last_position.y - position.y);
    const f32 z = (f32)((f64)mi->last_position.z - position.z);

    const f32 length_squared = (f32)((f64)x * x + (f64)y * y + (f64)z * z);

    mi->delta_length  = (f32)std::sqrt((f64)length_squared);
    mi->last_position = position;
}

// sub_602830
// inlined @ sub_611A90
// inlined @ sub_602CF0
// inlined @ sub_613D10
// inlined @ sub_614E90
ai_core* actor::get_ai_core() const {
    return my_base_ai_data ? my_base_ai_data->core : nullptr;
}

// sub_602F30
als::animation_logic_system_interface* actor::get_als() const {
    if (adv_ptrs && adv_ptrs->my_als_data)
        return adv_ptrs->my_als_data->the_als;

    return nullptr;
}

// sub_602580
bool actor::has_entity_collision() const {
    return colgeom && (flags & entity_flag_collisions_active) && (colgeom->flags & fi_collide_entity);
}

// sub_6025B0
bool actor::has_camera_collision() const {
    return colgeom && (flags & entity_flag_collisions_active) && (colgeom->flags & fi_collide_camera);
}

// sub_602790
void actor::replace_colgeom(collision_geometry* new_colgeom) { // a dynamic one is deleted
    if (colgeom && colgeom->is_dynamic())
        delete colgeom;

    colgeom = new_colgeom;
}

void actor::set_frame_delta_no_update(const po &bob, f32 t) {
    retail::sub_629340((u32*)this, (f32*)&bob, t);
}

// sub_6299F0
void actor::save_last_collision_free_state() { // a dynamic clone keeps one per member
    auto clone = is_a_dynamic_conglomerate_clone() ? (dynamic_conglomerate_clone*)this : nullptr;

    const po* matrix = clone ? clone->my_abs_po : my_abs_po;

    if (!get_colgeom())
        return;

    capsule rel_cap(references::unk_00ffec30.get(), references::yvec.get(), 1.0f);

    if (get_colgeom()->get_type() == collision_geometry_type_capsule)
        rel_cap = ((collision_capsule*)get_colgeom())->rel_cap;

    if (clone)
        clone->save_last_collision_free_state(this, *matrix, rel_cap);
    else
        save_last_collision_free_state(*matrix, rel_cap);
}

// sub_6150C0
void actor::save_last_collision_free_state(const po &matrix, const capsule &rel_cap) {
    if (!last_collision_free_state)
        last_collision_free_state = new (mm::allocate(sizeof(last_collision_free_state_t))) last_collision_free_state_t();

    last_collision_free_state->xform   = matrix;
    last_collision_free_state->rel_cap = rel_cap;

    if (colgeom)
        colgeom->get_type(); // asked and dropped
}

// sub_620ED0
void actor::create_adv_ptrs() {
    if (adv_ptrs)
        return;

    adv_ptrs = new (mm::allocate(sizeof(advanced_entity_ptrs))) advanced_entity_ptrs();

    ext_flags |= entity_ext_flag_alloc_adv_ptrs;
}

// inlined @ sub_620F50
// inlined @ sub_620FC0
// inlined @ sub_621040
// inlined @ sub_6653F0
render_info* actor::create_render_info() {
    create_adv_ptrs();

    if (!adv_ptrs->m_render_info)
        adv_ptrs->m_render_info = new (mm::allocate(sizeof(render_info))) render_info();

    return adv_ptrs->m_render_info;
}

// sub_65B290
void actor::add_collision_ignorance(arch_base_vhandle ent) { // stale handles met on the way are dropped
    engine_lock_scope lock(&jq::references::unk_01126d10.get());

    if (ent.null())
        return;

    create_adv_ptrs();

    if (!adv_ptrs->ignore_col_ents) {
        adv_ptrs->ignore_col_ents = new (mm::allocate(sizeof(vc8::vector<arch_base_vhandle>))) vc8::vector<arch_base_vhandle>();

        adv_ptrs->ignore_col_ents->reserve(2);
        adv_ptrs->ignore_col_ents->clear();
    }

    bool found = false;

    arch_base_vhandle* it = adv_ptrs->ignore_col_ents->begin();

    while (it != adv_ptrs->ignore_col_ents->end()) {
        if (it->resolve()) {
            if (*it == ent)
                found = true;

            ++it;
        } else
            adv_ptrs->ignore_col_ents->erase(it);
    }

    if (!found && ent.resolve())
        adv_ptrs->ignore_col_ents->push_back(ent);
}

// sub_64B1D0
void actor::remove_collision_ignorance(arch_base_vhandle ent) { // stale handles met on the way are dropped too
    engine_lock_scope lock(&jq::references::unk_01126d10.get());

    if (!adv_ptrs || !adv_ptrs->ignore_col_ents || adv_ptrs->ignore_col_ents->empty())
        return;

    arch_base_vhandle* it = adv_ptrs->ignore_col_ents->begin();

    while (it != adv_ptrs->ignore_col_ents->end()) {
        if (*it == ent || !it->resolve())
            adv_ptrs->ignore_col_ents->erase(it);
        else
            ++it;
    }
}

// sub_614190
void actor::get_velocity(vector3d* target) const {
    const entity_base* ancestor = my_parent;

    if (!ancestor) {
        *target = has_physical_ifc() ? physical_ifc()->get_velocity() : references::unk_00ffec30.get();

        if (adv_ptrs && adv_ptrs->mi && adv_ptrs->mi->frame_delta_valid)
            adv_ptrs->mi->add_frame_delta_velocity(target);

        return;
    }

    if (has_physical_ifc()) {
        *target = physical_ifc()->get_velocity();

        entity* control_parent = has_standing_ifc() ? standing_ifc()->get_control_parent() : nullptr;

        if (adv_ptrs &&
            adv_ptrs->mi &&
            adv_ptrs->mi->frame_delta_valid &&
            control_parent == my_parent)

            adv_ptrs->mi->add_frame_delta_velocity(target);
    } else
        *target = references::unk_00ffec30.get();

    // the rel po carried through every ancestor but the root
    matrix4x4 carried = my_rel_po->get_matrix();

    while (ancestor->my_parent) {
        const matrix4x4 &abs_matrix = ancestor->my_abs_po->get_matrix();

        matrix4x4 product;

        for (i32 i = 0; i < 4; i++)
            for (i32 j = 0; j < 4; j++)
                product[i][j] = (f32)((f64)abs_matrix.x[j] * carried[i].x +
                                      (f64)abs_matrix.y[j] * carried[i].y +
                                      (f64)abs_matrix.z[j] * carried[i].z +
                                      (f64)abs_matrix.w[j] * carried[i].w);

        carried  = product;
        ancestor = ancestor->my_parent;
    }

    if (!ancestor->is_an_actor())
        return;

    vector3d root_velocity = references::unk_00ffec30.get();

    ((const actor*)ancestor)->get_velocity(&root_velocity);

    vector3d angular_velocity = references::unk_00ffec30.get();

    if (ancestor->has_physical_ifc())
        angular_velocity = ancestor->physical_ifc()->angular_velocity;

    const vector4d &lever = carried.w;

    const f32 cross_x = (f32)((f64)angular_velocity.z * lever.y - (f64)angular_velocity.y * lever.z);
    const f32 cross_y = (f32)((f64)lever.z * angular_velocity.x - (f64)angular_velocity.z * lever.x);
    const f32 cross_z = (f32)((f64)angular_velocity.y * lever.x - (f64)lever.y * angular_velocity.x);

    const f32 x = (f32)((f64)root_velocity.x + cross_x);
    const f32 y = (f32)((f64)root_velocity.y + cross_y);
    const f32 z = (f32)((f64)root_velocity.z + cross_z);

    target->x = (f32)((f64)target->x + x);
    target->y = (f32)((f64)target->y + y);
    target->z = (f32)((f64)target->z + z);
}
