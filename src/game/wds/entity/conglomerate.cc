#include <cmath>
#include <cstdlib>
#include <new>

#include "retail.hh"
#include "game/math/references.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/limbo.hh"
#include "game/wds/entity/moved_entities.hh"
#include "game/binary_search.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/ngl/mesh/mesh.hh"

using namespace treyarch;

string_hash conglomerate::my_type_key { 0x6DD7D480 }; // "conglomerate"

// sub_665490
conglomerate::~conglomerate() {
    finalize(mash::ALLOCATED);
}

void conglomerate::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(conglomerate*, mash::allocation_scope))retail::sub_661D70)(this, scope);
}

// sub_65B7C0
void conglomerate::initialize(mash::allocation_scope scope) {
    lightmgr = nullptr;

    if (scope == mash::ALLOCATED) {
        conglomerate_flags          = 0;
        conglomerate_visual_radius  = -1.0f;
        conglomerate_colgeom_radius = -1.0f;
        skeleton_name_hash          = string_hash();
        masher_skin_bones           = nullptr;
        collision_members           = nullptr;
        light_members               = nullptr;
        bone_connectors             = nullptr;
        tentacles                   = nullptr;
        attach_index                = invalid_conglom_member;
    } else {
        create_parentage_tree();

        ngl::mesh* my_mesh = unk_111();

        if (my_mesh && my_mesh->skeleton && my_mesh->skeleton->unk_008)
            flags |= entity_flag_active;

        if (!is_a_vehicle())
            ext_flags |= entity_ext_flag_recordable;

        start_recording();
    }

    conglomerate_flags |= conglomerate_flag_visual_radius_dirty | conglomerate_flag_colgeom_radius_dirty;

    conglomerate_visual_radius  = -1.0f;
    conglomerate_colgeom_radius = -1.0f;

    if (has_variant_ifc())
        references::random_variant_queue.get().push_back(my_handle);
}

// sub_661C50
void conglomerate::construct_mashed_class() {
    actor::construct_mashed_class();

    members      .construct_mashed_class();
    member_abs_po.construct_mashed_class();
    all_rel_po   .construct_mashed_class();
    all_model_po .construct_mashed_class();

    original_rel_po.construct_mashed_class();
    member_parents .construct_mashed_class();

    root_index.name.construct_mashed_class();

    bone_indices.construct_mashed_class();
    unk_154     .construct_mashed_class();

    skeleton_to_conglom_bone_mapping   .construct_mashed_class();
    unassigned_skin_bone_rel_pos       .construct_mashed_class();
    unassigned_skin_bone_indices       .construct_mashed_class();
    unassigned_skin_bone_parent_indices.construct_mashed_class();

    if (tentacles)
        retail::sub_6515A0((void**)&tentacles); // the tentacle vector's construct_mashed_class

    initialize(mash::FROM_MASH);
}

// slot 2
void conglomerate::destruct_mashed_class() {
    ((void (__thiscall*)(conglomerate*))retail::sub_667BA0)(this);
}

// slot 3
void conglomerate::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(conglomerate*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_65B5D0)(this,
                                                                                                               mash_info,
                                                                                                               containing_class_ptr,
                                                                                                               buffer);
}

// slot 4
mash::virtual_types_key conglomerate::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const conglomerate*))retail::sub_665660)(this);
}

// slot 6
bool conglomerate::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const conglomerate*, mash::virtual_types_key))retail::sub_71C790)(this, parent_class);
}

// slot 8
i32 conglomerate::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const conglomerate*))retail::sub_665650)(this);
}

// slot 13
bool conglomerate::is_a_conglomerate() const {
    return ((bool (__thiscall*)(const conglomerate*))retail::sub_667F80)(this);
}

// sub_6319A0
void conglomerate::set_flag_recursive(u32 flag, bool on) { // a connector keeps its own flags, its members still get the call
    if (!(flags & entity_flag_bone_connector)) {
        if (on)
            flags |= flag;
        else
            flags &= ~flag;
    }

    for (entity_base* member : members)
        member->set_flag_recursive(flag, on);
}

// sub_631A10
void conglomerate::set_ext_flag_recursive(u32 flag, bool on) { // same
    if (!(flags & entity_flag_bone_connector)) {
        if (on)
            ext_flags |= flag;
        else
            ext_flags &= ~flag;
    }

    for (entity_base* member : members)
        member->set_ext_flag_recursive(flag, on);
}

// sub_645FD0
void conglomerate::set_visible(bool visible, bool from_conglom) {
    const bool was_visible = is_visible();

    for (entity_base* member : members) {
        if (member->bone_idx != 0xFF)
            continue;

        if (!(member->flags & entity_flag_member_hide))
            member->set_visible(visible, true);
        else if (was_visible != visible)
            member->set_visible(false, true);
    }

    if (is_visible() == visible)
        return;

    if (visible)
        flags |= entity_flag_graphics_visible;
    else
        flags &= ~entity_flag_graphics_visible;

    if (!is_visible())
        occlusion_status = references::occlusion_status_base.read() - 1;

    if (flags & entity_flag_member) {
        conglomerate* owner = get_conglom_owner();

        if (owner) {
            owner->update_proximity_maps();

            owner->conglomerate_flags = (owner->conglomerate_flags & ~conglomerate_flag_possibly_render_valid) |
                                        conglomerate_flag_visual_radius_dirty;
        }
    } else {
        if (is_a_conglomerate())
            conglomerate_flags = (conglomerate_flags & ~conglomerate_flag_possibly_render_valid) |
                                 conglomerate_flag_visual_radius_dirty;

        update_proximity_maps();
    }

    if (has_manip_obj_ifc())
        manip_obj_ifc()->force_status_update();
}

// slot 77
e_entity_flavor conglomerate::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const conglomerate*))retail::sub_AD6450)(this);
}

// slot 86
void conglomerate::post_unmash_fixup(amalga::resource_pack_slot* pack_slot) {
    ((void (__thiscall*)(conglomerate*, amalga::resource_pack_slot*))retail::sub_6562D0)(this, pack_slot);
}

// slot 87
u32 conglomerate::get_entity_size() const {
    return ((u32 (__thiscall*)(const conglomerate*))retail::sub_665650)(this);
}

// sub_63AE70
void conglomerate::clear_region(region* r, i32 i_know_what_i_am_doing) {
    remove_me_from_region(r);

    limbo::update_limbo_entity(this);

    remove_member_lights_from_region(r);
}

// sub_6168E0
void conglomerate::force_region(region* r) {
    entity::force_region(r);
}

// sub_6168F0
void conglomerate::force_regions(entity* e) {
    entity::force_regions(e);
}

// sub_631AE0
u32 conglomerate::possibly_render() const { // answers the flag's bit, not 1
    if (conglomerate_flags & conglomerate_flag_possibly_render_valid)
        return conglomerate_flags & conglomerate_flag_possibly_render;

    if (is_still_visible() && (flags & entity_flag_graphics)) {
        conglomerate_flags |= conglomerate_flag_possibly_render_valid | conglomerate_flag_possibly_render;

        return conglomerate_flag_possibly_render;
    }

    for (entity_base* member : members) {
        if (member->is_an_entity() && ((entity*)member)->possibly_render()) {
            conglomerate_flags |= conglomerate_flag_possibly_render_valid | conglomerate_flag_possibly_render;

            return conglomerate_flag_possibly_render;
        }
    }

    conglomerate_flags = (conglomerate_flags & ~conglomerate_flag_possibly_render) | conglomerate_flag_possibly_render_valid;

    return 0;
}

// sub_617640
u32 conglomerate::possibly_collide() const {
    if (has_collision_members())
        return 1;

    return get_colgeom() && (flags & entity_flag_collisions_active);
}

// sub_63AEE0
bool conglomerate::possibly_camera_collide() const {
    if (get_colgeom() && (flags & entity_flag_collisions_active) && (get_colgeom()->flags & fi_collide_camera))
        return true;

    if (!has_collision_members())
        return false;

    for (actor* member : *collision_members) {
        if (member->possibly_camera_collide())
            return true;
    }

    return false;
}

// sub_63AF70
bool conglomerate::possibly_walkable() const {
    if (get_colgeom() && (flags & entity_flag_collisions_active) && (flags & entity_flag_walkable))
        return true;

    if (!has_collision_members())
        return false;

    for (actor* member : *collision_members) {
        if (member->possibly_walkable())
            return true;
    }

    return false;
}

// sub_617690
void conglomerate::region_update_poss_collide() {
    entity::region_update_poss_collide();
}

// sub_62F810
bool conglomerate::is_still_visible() const {
    if (is_visible())
        return true;

    for (entity_base* member : members) {
        if (member->is_an_entity() && ((entity*)member)->is_still_visible())
            return true;
    }

    return false;
}

// slot 108
void conglomerate::unk_108(f32 unk) {
    ((void (__thiscall*)(conglomerate*, f32))retail::sub_64DAE0)(this, unk);
}

// slot 109
void conglomerate::render(f32 camera_distance, f32 entity_translucency_pct) {
    ((void (__thiscall*)(conglomerate*, f32, f32))retail::sub_630690)(this, camera_distance, entity_translucency_pct);
}

// slot 113
f32 conglomerate::get_visual_radius() const {
    return ((f32 (__thiscall*)(const conglomerate*))retail::sub_63A830)(this);
}

// sub_63A660
vector3d conglomerate::get_visual_center() const {
    if (attach_index == invalid_conglom_member)
        return actor::get_visual_center();

    po attach_po;
    attach_po.identity();

    get_member_abs_po(attach_index, &attach_po);

    vector3d attach_position;
    get_member_abs_position(attach_index, &attach_position);

    if (!get_mesh())
        return attach_position;

    if (ext_flags & entity_ext_flag_bounding_sphere_dirty) {
        const math::vector4 &sphere = get_mesh()->sphere;

        const f32 x = sphere.get_x();
        const f32 y = sphere.get_y();
        const f32 z = sphere.get_z();

        const matrix4x4 &m = attach_po.get_matrix();

        cached_visual_bounding_sphere_center = vector3d((f32)((f64)m.y.x * y + (f64)m.x.x * x + (f64)m.z.x * z + m.w.x),
                                                        (f32)((f64)m.y.y * y + (f64)m.x.y * x + (f64)m.z.y * z + m.w.y),
                                                        (f32)((f64)m.y.z * y + (f64)m.x.z * x + (f64)m.z.z * z + m.w.z));

        ext_flags &= ~entity_ext_flag_bounding_sphere_dirty;
    }

    return cached_visual_bounding_sphere_center;
}

// sub_62F670
vector3d conglomerate::unk_115() { // without a mesh, the mean of the visible actor members' centers
    if (!get_mesh()) {
        vector3d sum   = references::unk_00ffec30.get();
        i32      count = 0;

        for (entity_base* member : members) {
            if (!member->is_an_actor() || !member->is_visible())
                continue;

            auto* ent = (entity*)member;

            if (!ent->unk_111())
                continue;

            const vector3d center = ent->get_visual_center();

            ++count;

            sum.x = (f32)((f64)center.x + sum.x);
            sum.y = (f32)((f64)center.y + sum.y);
            sum.z = (f32)((f64)center.z + sum.z);
        }

        if (count) {
            const f32 divisor = (f32)count;

            return vector3d((f32)((f64)sum.x / divisor),
                            (f32)((f64)sum.y / divisor),
                            (f32)((f64)sum.z / divisor));
        }
    }

    return actor::get_visual_center();
}

// sub_6305A0
void conglomerate::set_render_color(color32 color) { // members on a bone are skipped
    actor::set_render_color(color);

    for (entity_base* member : members) {
        if (member->is_an_entity() && member->bone_idx == 0xFF)
            ((entity*)member)->set_render_color(color);
    }
}

// sub_630610
void conglomerate::set_render_alpha_mod(f32 alpha) { // same
    actor::set_render_alpha_mod(alpha);

    for (entity_base* member : members) {
        if (member->is_an_entity() && member->bone_idx == 0xFF)
            ((entity*)member)->set_render_alpha_mod(alpha);
    }
}

// sub_631C00
void conglomerate::set_render_scale(const vector3d &scale) { // a big enough change dirties the visual radius
    const vector3d wanted = scale;
    const vector3d old    = get_render_scale();

    const f32 x = (f32)((f64)old.x - wanted.x);
    const f32 y = (f32)((f64)old.y - wanted.y);
    const f32 z = (f32)((f64)old.z - wanted.z);

    const bool changed = (f32)((f64)x * x + (f64)y * y + (f64)z * z) > 0.01f;

    actor::set_render_scale(scale);

    if (changed)
        conglomerate_flags |= conglomerate_flag_visual_radius_dirty;
}

// sub_603DB0
light_manager* conglomerate::get_light_set() const {
    return lightmgr;
}

// sub_631B90
void conglomerate::set_recursive_age(f32 age) {
    set_age(age);

    for (entity_base* member : members) {
        if (member->is_an_entity())
            ((entity*)member)->set_recursive_age(age);
    }
}

// sub_631A70
void conglomerate::set_colgeom_flag_recursive(u32 flag, bool set) {
    actor::set_colgeom_flag_recursive(flag, set);

    for (entity_base* member : members) {
        if (member->is_an_actor())
            ((actor*)member)->set_colgeom_flag_recursive(flag, set);
    }
}

// sub_63AC20
f32 conglomerate::get_colgeom_radius() const {
    if (conglomerate_flags & conglomerate_flag_colgeom_radius_dirty) {
        conglomerate_colgeom_radius = colgeom ? colgeom->get_bounding_sphere_radius() : 0.0f;

        if (has_collision_members()) {
            for (actor* member : *collision_members) {
                const vector3d center        = get_colgeom_center();
                const vector3d member_center = member->get_colgeom_center();

                const f32 x = (f32)((f64)member_center.x - center.x);
                const f32 y = (f32)((f64)member_center.y - center.y);
                const f32 z = (f32)((f64)member_center.z - center.z);

                const f32 member_radius = member->get_colgeom_radius();
                const f32 reach         = std::sqrt(x * x + y * y + z * z) + member_radius;

                if (reach > conglomerate_colgeom_radius)
                    conglomerate_colgeom_radius = reach;
            }

            conglomerate_colgeom_radius = (f32)((f64)conglomerate_colgeom_radius * (f64)1.1f);
        }

        conglomerate_flags &= ~conglomerate_flag_colgeom_radius_dirty;
    }

    return conglomerate_colgeom_radius;
}

// sub_665670
vector3d conglomerate::get_colgeom_center() const {
    if (get_colgeom())
        return actor::get_colgeom_center();

    return my_abs_po->get_position();
}

// sub_603C30
void conglomerate::radius_changed(bool force) {
    conglomerate_flags |= conglomerate_flag_visual_radius_dirty |
                          conglomerate_flag_colgeom_radius_dirty |
                          conglomerate_flag_unk_00004000;

    ext_flags |= entity_ext_flag_bounding_sphere_dirty;
}

// sub_616A10
void conglomerate::compute_sector(terrain* ter, bool use_high_res_intersect) {
    if (!(flags & entity_flag_region_forced))
        moved_entities::add_moved(my_handle);
}

// sub_665640
void conglomerate::frame_done() {
    frame_done_including_members();
}

// slot 160
void conglomerate::frame_done_including_members() {
    ((void (__thiscall*)(conglomerate*))retail::sub_5B4A70)(this);
}

// sub_62F880
e_conglom_member_index conglomerate::get_member_index(const string_hash &nodename) const {
    // a bone miss leaves its insertion point for the members search
    i32 found = -1;

    if (bone_indices.size() &&
        binary_search_array(nodename, bone_indices.get_vector().array, (i32)bone_indices.size(), &found))

        return (e_conglom_member_index)(members.size + found);

    if (members.size && binary_search_array_deref(nodename, members.array, (i32)members.size, &found))
        return (e_conglom_member_index)found;

    return nodename == id ? base_conglom_index : invalid_conglom_member;
}

// sub_616950
// inlined @ sub_6366D0
entity_base* conglomerate::get_member(e_conglom_member_index index) const {
    if (index >= 0 && (u32)index < members.size)
        return members.array[index];

    return index == base_conglom_index ? (entity_base*)this : nullptr;
}

// sub_6366D0
entity_base* conglomerate::get_member(const string_hash &nodename) const {
    return get_member(get_member_index(nodename));
}

// sub_62F970
const string_hash &conglomerate::get_member_id(e_conglom_member_index index) const {
    if (index >= 0) {
        if ((u32)index < members.size)
            return members.array[index]->id;

        i32 slot = (i16)(index - members.size);

        if ((u32)slot < bone_indices.size())
            return bone_indices.get_vector().array[slot].name;
    } else if (index == base_conglom_index)
        return id;

    static string_hash no_id;

    no_id = string_hash();

    return no_id;
}

// sub_6169D0
bool conglomerate::has_member(const string_hash &nodename) const {
    if (!members.size)
        return false;

    i32 index;

    return binary_search_array_deref(nodename, members.array, (i32)members.size, &index);
}

// sub_40A810
bool conglomerate::has_member_for_linking(const string_hash &nodename) const {
    e_conglom_member_index index = get_member_index(nodename);

    return index >= 0 && (u32)index < bone_indices.size() + members.size;
}

// sub_655D80
entity_base* conglomerate::get_member_for_linking(e_conglom_member_index index) {
    if (index < 0)
        return index == base_conglom_index ? this : nullptr;

    if ((u32)index < members.size)
        return members.array[index];

    i32 slot = (i16)(index - members.size);

    if ((u32)slot >= bone_indices.size())
        return nullptr;

    const conglomerate_bone_index &bone = bone_indices.get_vector().array[slot];

    i32 found = -1;

    if (bone_connectors && !bone_connectors->empty()) {
        if (binary_search_array_deref(bone.name, bone_connectors->begin(), (i32)bone_connectors->size(), &found))
            return bone_connectors->begin()[found];
    }

    entity_base* connector = nullptr;

    if (void* memory = mm::allocate(sizeof(entity_base)))
        connector = new (memory) entity_base(bone.name, entity_flag_bone_connector | entity_flag_member);

    connector->bone_idx  = (u8)bone.model_po_index;
    connector->my_rel_po = &all_model_po.array[connector->bone_idx];

    auto* connector_abs_po = (po*)mm::allocate(sizeof(po));

    if (connector_abs_po)
        connector_abs_po->identity();

    connector->my_abs_po       = connector_abs_po;
    connector->my_conglom_root = this;
    connector->set_parent(this);

    if (!bone_connectors)
        bone_connectors = new (mm::allocate(sizeof(vc8::vector<entity_base*>))) vc8::vector<entity_base*>();

    bone_connectors->push_back(connector);

    if (bone_connectors->size() > 1)
        std::qsort(bone_connectors->begin(),
                   bone_connectors->size(),
                   sizeof(entity_base*),
                   compare_entity_base_ptr_entity_id);

    return connector;
}

// sub_408F10
entity_base* conglomerate::get_member_for_linking(const string_hash &nodename) {
    return get_member_for_linking(get_member_index(nodename));
}

// sub_62FA60
bool conglomerate::get_member_abs_position(e_conglom_member_index index, vector3d* pos, const po* model_po_override) const {
    if (index < 0) {
        if (index != base_conglom_index)
            return false;

        *pos = my_abs_po->get_position();

        return true;
    }

    if ((u32)index < members.size) {
        *pos = members.array[index]->my_abs_po->get_position();

        return true;
    }

    i32 slot = (i16)(index - members.size);

    if ((u32)slot >= bone_indices.size())
        return false;

    const po*        model_pos = model_po_override ? model_po_override : all_model_po.array;
    const vector3d  &local     = model_pos[bone_indices.get_vector().array[slot].model_po_index].get_position();
    const matrix4x4 &m         = my_abs_po->get_matrix();

    *pos = vector3d((f32)((f64)m.y.x * local.y + (f64)m.x.x * local.x + (f64)m.z.x * local.z + (f64)m.w.x),
                    (f32)((f64)m.y.y * local.y + (f64)m.x.y * local.x + (f64)m.z.y * local.z + (f64)m.w.y),
                    (f32)((f64)m.y.z * local.y + (f64)m.x.z * local.x + (f64)m.z.z * local.z + (f64)m.w.z));

    return true;
}

// sub_402560
bool conglomerate::get_member_abs_position(const string_hash &nodename, vector3d* pos, const po* model_po_override) const {
    return get_member_abs_position(get_member_index(nodename), pos, model_po_override);
}

// sub_62FC10
bool conglomerate::get_member_abs_po(e_conglom_member_index index, po* the_po, const po* model_po_override) const {
    if (index < 0) {
        if (index != base_conglom_index)
            return false;

        *the_po = *my_abs_po;

        return true;
    }

    if ((u32)index < members.size) {
        *the_po = *members.array[index]->my_abs_po;

        return true;
    }

    i32 slot = (i16)(index - members.size);

    if ((u32)slot >= bone_indices.size())
        return false;

    const po* model_pos = model_po_override ? model_po_override : all_model_po.array;

    fast_po_mul(*the_po, model_pos[bone_indices.get_vector().array[slot].model_po_index], *my_abs_po);

    return true;
}

// sub_402590
bool conglomerate::get_member_abs_po(const string_hash &nodename, po* the_po, const po* model_po_override) const {
    return get_member_abs_po(get_member_index(nodename), the_po, model_po_override);
}

// sub_640EA0
bool conglomerate::set_member_abs_position(e_conglom_member_index index, const vector3d &pos) {
    if (index < 0) {
        if (index != base_conglom_index)
            return false;

        entity_set_abs_position(this, pos);

        return true;
    }

    if ((u32)index < members.size) {
        entity_set_abs_position(members.array[index], pos);

        return true;
    }

    i32 slot = (i16)(index - members.size);

    if ((u32)slot >= bone_indices.size())
        return false;

    const conglomerate_bone_index &bone = bone_indices.get_vector().array[slot];

    vector3d model_position = all_model_po.array[bone.model_po_index].get_position();
    vector3d target         = pos;

    po               inverse = my_abs_po->inverse();
    const matrix4x4 &m       = inverse.get_matrix();

    // the target in model space, each component rounded before the bone's position comes off it
    f32 x = (f32)((f64)m.y.x * target.y + (f64)m.x.x * target.x + (f64)m.z.x * target.z + (f64)m.w.x);
    f32 y = (f32)((f64)m.y.y * target.y + (f64)m.x.y * target.x + (f64)m.z.y * target.z + (f64)m.w.y);
    f32 z = (f32)((f64)m.y.z * target.y + (f64)m.x.z * target.x + (f64)m.z.z * target.z + (f64)m.w.z);

    vector3d delta((f32)((f64)x - model_position.x),
                   (f32)((f64)y - model_position.y),
                   (f32)((f64)z - model_position.z));

    translate_member_model_position_recurse(bone, delta);
    dirty_bone_connectors(false);
    radius_changed(true);

    return true;
}

// sub_6410F0
bool conglomerate::set_member_abs_po(e_conglom_member_index index, const po &the_po, po* model_po_override) {
    if (index < 0) {
        if (index != base_conglom_index)
            return false;

        entity_set_abs_po(this, the_po);

        return true;
    }

    if ((u32)index < members.size) {
        entity_set_abs_po(members.array[index], the_po);

        return true;
    }

    i32 slot = (i16)(index - members.size);

    if ((u32)slot >= bone_indices.size())
        return false;

    const conglomerate_bone_index &bone = bone_indices.get_vector().array[slot];

    po old_inverse;
    old_inverse.identity();

    po* model_pos = model_po_override ? model_po_override : all_model_po.array;
    po* model_po  = &model_pos[bone.model_po_index];

    old_inverse = model_po->inverse();

    fast_po_mul(*model_po, the_po, my_abs_po->inverse());

    ((void (__thiscall*)(conglomerate*, const conglomerate_bone_index&, const po&, const po&, po*))retail::sub_62FEE0)(this, bone, *model_po, old_inverse, model_po_override); // conglomerate::update_member_model_po_recurse

    dirty_bone_connectors(false);
    radius_changed(true);

    return true;
}

// sub_48E860
bool conglomerate::set_member_abs_po(const string_hash &nodename, const po &the_po, po* model_po_override) {
    return set_member_abs_po(get_member_index(nodename), the_po, model_po_override);
}

// sub_6304B0
void conglomerate::translate_member_model_position_recurse(const conglomerate_bone_index &bone_index, const vector3d &delta) {
    vector4d &translation = all_model_po.array[bone_index.model_po_index].get_matrix().w;

    translation = vector4d((f32)((f64)translation.x + delta.x),
                           (f32)((f64)translation.y + delta.y),
                           (f32)((f64)translation.z + delta.z),
                           1.0f);

    e_conglom_member_index child = bone_index.child_index;

    while (child != invalid_conglom_member) {
        i32 slot = (i16)(child - members.size);

        translate_member_model_position_recurse(bone_indices.get_vector().array[slot], delta);

        child = bone_indices.get_vector().array[slot].sibling_index;
    }
}

// sub_63AEA0
void conglomerate::dirty_bone_connectors(bool force_entire_tree) {
    if (!bone_connectors)
        return;

    for (entity_base* connector : *bone_connectors)
        connector->update_abs_po_from_model_po();
}

// sub_656460
void conglomerate::create_parentage_tree() {
    for (i32 i = 0; i < (i32)members.size; ++i) {
        u8 parent = (u8)member_parents.get_vector().array[i];

        if (parent == 0xFF)
            members.array[i]->set_parent(this);
        else {
            entity_base* linked = get_member_for_linking((e_conglom_member_index)parent);

            members.array[i]->set_parent(linked);
        }
    }
}

// sub_63ADF0
void conglomerate::add_member_lights_to_region(region* the_region) {
    if (!light_members)
        return;

    for (light_source* light : *light_members)
        retail::sub_7A8580((u32*)the_region, (i32)light); // region::add_light
}

// sub_63AE30
void conglomerate::remove_member_lights_from_region(region* the_region) {
    if (!light_members)
        return;

    for (light_source* light : *light_members)
        retail::sub_7A3090((u32*)the_region, (i32)light); // region::remove_light
}

// sub_631CE0
void conglomerate::set_all_collisions_active(bool val) {
    set_collisions_active(val, true);

    for (entity_base* member : members)
        member->set_collisions_active(val, true);
}
